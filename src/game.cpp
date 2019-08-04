#include <algorithm>
#include <iostream>
#include <random>

#include "game.h"

// Speed in pixels per frame
int constexpr PLAYER_SPEED{4};
int constexpr BULLET_SPEED{8};
int constexpr MIN_ENEMY_SPEED{2};
int constexpr MAX_ENEMY_SPEED{6};
int constexpr BG_ROLLING_SPEED{1};

// Life in # of hits can tolerate
int constexpr PLAYER_LIFE{1};  // # of hits
int constexpr ENEMY_LIFE{1};

// Time in # of frames
int constexpr PLAYER_RELOAD{8};
int constexpr MIN_SPWAN_TIME{30};
int constexpr MAX_SPWAN_TIME{90};
int constexpr MIN_FIRE_TIME{60};
int constexpr MAX_FIRE_TIME{180};

// Explosion
int constexpr EXPLOSION_PIC_COUNT{10};
int constexpr EXPLOSION_PIC_INTERVAL{12};

Game::Game(Renderer &renderer, Controller &controller) 
    : renderer{renderer}, controller{controller}, enemySpwanTimer{90} 
{}

void Game::Run(std::size_t target_frame_duration) {
    bool running = true;
    bool game_over = false;
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    Uint32 title_timestamp = SDL_GetTicks();
    int frame_count = 0;

    // Random generator for enemy spwan, enemy speed, 
    // enemy position, and enemy fire
    std::random_device rd;
    std::mt19937 eng{rd()};

    // Pre-load game objects images
    PreloadTextures();

    // Initialize background
    Background bg{background_texture, BG_ROLLING_SPEED, 
                  renderer.GetScreenRect()};

    // Initialize player
    Fighter player{player_texture, PLAYER_SPEED, PLAYER_LIFE};
    player.SetPosition(100, 100);

    std::forward_list<Bullet> player_bullets;

    // Initialize enemies
    std::forward_list<Fighter> enemies;
    std::forward_list<Bullet> enemy_bullets;

    // Initialize explosions
    std::forward_list<Explosion> explosions;

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        auto actions = controller.ProcessEvent();
        running = !actions.QUIT;

        if (!game_over) {
            // Game objects update
            bg.Rolling();
            UpdatePlayerObjects(actions, player, player_bullets);
            UpdateEnemyObjects(enemies, enemy_bullets, eng);
            UpdateExplosions(explosions);
            BulletsHitEnemies(player_bullets, enemies, explosions);
            BulletsHitBullets(player_bullets, enemy_bullets);
            BulletsHitPlayer(enemy_bullets, player);
            EnemiesHitPlayer(enemies, player);
            ClearInvalidObjects(enemies, player_bullets, enemy_bullets);
            game_over = player.IsDead();
        }

        RenderScreen(bg, player, player_bullets, enemies, enemy_bullets, explosions);
        
        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render 
        // cycle tasks.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000) {
            renderer.UpdateWindowTitle(frame_count);
            frame_count = 0;
            title_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is 
        // smaller than the target_frame_duration), delay the loop to achieve 
        // the correct frame rate.
        if (frame_duration < target_frame_duration) {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}

void Game::RenderScreen(Background &bg, Fighter &player,
                        std::forward_list<Bullet> &player_bullets,
                        std::forward_list<Fighter> &enemies,
                        std::forward_list<Bullet> &enemy_bullets,
                        std::forward_list<Explosion> &explosions) {
    renderer.BeginRender();

    // Render Background
    bg.Render(renderer);

    // Render player
    player.Render(renderer);

    // Render player bullets
    for (auto &bullet : player_bullets) {
        bullet.Render(renderer);
    }

    // Render enemies
    for (auto &enemy : enemies) {
        enemy.Render(renderer);
    }

    // Render enemy bullets
    for (auto &e_bullet : enemy_bullets) {
        e_bullet.Render(renderer);
    }

    // Render explosions
    for (auto &explosion : explosions) {
        explosion.Render(renderer);
    }

    renderer.EndRender();
}

void Game::UpdatePlayerObjects(Controller::Actions const &actions, 
                               Fighter &player, 
                               std::forward_list<Bullet> &bullets) {
    SDL_Rect bound_rect = renderer.GetScreenRect();

    // Move player
    player.Move(actions.UP, actions.DOWN, actions.LEFT, actions.RIGHT);
    player.BoundAdjust(bound_rect);

    // Move flying bullets
    for (auto &b : bullets) {
        b.Move(false, false, false, true);
    }

    // Fire new bullet
    if (player.Fire(actions.FIRE)) {
        Bullet new_bullet{player_bullet_texture, BULLET_SPEED};
        SDL_Rect player_rect_new = player.GetRect();
        SDL_Rect bullet_rect = new_bullet.GetRect();
        int bullet_x = player_rect_new.x + player_rect_new.w;
        int bullet_y = player_rect_new.y + 
                       (player_rect_new.h - bullet_rect.h) / 2;
        new_bullet.SetPosition(bullet_x, bullet_y);
        bullets.push_front(std::move(new_bullet));
        player.Reload(PLAYER_RELOAD);
    }
}

void Game::UpdateEnemyObjects(std::forward_list<Fighter> &enemies, 
                                  std::forward_list<Bullet> &bullets,
                                  std::mt19937 &eng) {
    SDL_Rect bound_rect = renderer.GetScreenRect();

    // Move flying enemies
    for (auto &e : enemies) {
        e.Move(false, false, true, false);
    }

    // Move flying bullets
    for (auto &b : bullets) {
        b.Move(false, false, true, false);
    }

    // Enemy will fire randomly in between defined range
    std::uniform_int_distribution<int> random_fire(MIN_FIRE_TIME, 
                                                   MAX_FIRE_TIME);

    // Spwan new enemy
    if (--enemySpwanTimer <= 0) {
        std::uniform_int_distribution<int> random_spwan(MIN_SPWAN_TIME, 
                                                        MAX_SPWAN_TIME);
        std::uniform_int_distribution<int> random_speed(MIN_ENEMY_SPEED, 
                                                        MAX_ENEMY_SPEED);

        Fighter new_enemy{enemy_texture, random_speed(eng), ENEMY_LIFE, 
                          random_fire(eng)};

        int max_y = bound_rect.h - new_enemy.GetRect().h;

        std::uniform_int_distribution<int> random_position(0, max_y);
        int pos_y = random_position(eng);

        if (pos_y > max_y) {
            pos_y = max_y;
        }
        new_enemy.SetPosition(bound_rect.w, random_position(eng));
        enemies.push_front(std::move(new_enemy));

        enemySpwanTimer = random_spwan(eng);
    }

    // Fire new bullets
    for (auto &enemy : enemies) {
        SDL_Rect enemy_rect = enemy.GetRect();

        if (enemy.Fire(true)) {
            Bullet new_bullet{enemy_bullet_texture, BULLET_SPEED};
            SDL_Rect bullet_rect = new_bullet.GetRect();
            int bullet_x = enemy_rect.x - bullet_rect.w;
            int bullet_y = enemy_rect.y + (enemy_rect.h - bullet_rect.h) / 2;
            new_bullet.SetPosition(bullet_x, bullet_y);
            bullets.push_front(std::move(new_bullet));
            enemy.Reload(random_fire(eng));
        }        
    }
}

void Game::UpdateExplosions(std::forward_list<Explosion> &explosions) {
    for (auto & explosion : explosions) {
        explosion.UpdateCounter();
    }

    explosions.remove_if([](Explosion &e) {
        return e.Disappeared();
    });
}

void Game::PreloadTextures() {
    std::string const PLAYER_IMAGE{"../gfx/player.png"};
    std::string const PLAYER_BULLET_IMAGE{"../gfx/playerBullet.png"};
    std::string const ENEMY_IMAGE("../gfx/enemy.png");
    std::string const ENEMY_BULLET_IMAGE("../gfx/enemyBullet.png");
    std::string const BACKGROUND_IMAGE("../gfx/background.png");
    std::string const EXPLOSIONS_IMAGE("../gfx/explosions.png");

    player_texture = renderer.LoadImage(PLAYER_IMAGE);
    player_bullet_texture = renderer.LoadImage(PLAYER_BULLET_IMAGE);
    enemy_texture = renderer.LoadImage(ENEMY_IMAGE);
    enemy_bullet_texture = renderer.LoadImage(ENEMY_BULLET_IMAGE);
    background_texture = renderer.LoadImage(BACKGROUND_IMAGE);
    explosions_texture = renderer.LoadImage(EXPLOSIONS_IMAGE);
}

bool Game::CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2) {
    return (std::max(rect1.x, rect2.x) 
                < std::min(rect1.x + rect1.w, rect2.x + rect2.w)) && 
           (std::max(rect1.y, rect2.y) 
                < std::min(rect1.y + rect1.h, rect2.y + rect2.h));
}

void Game::BulletsHitEnemies(std::forward_list<Bullet> &bullets, 
                            std::forward_list<Fighter> &enemies,
                            std::forward_list<Explosion> &explosions) {
    for (auto &b : bullets) {
        if (!b.IsHit()) {
            for (auto &e : enemies) {
                if (!e.IsDead() && CheckCollision(b.GetRect(), e.GetRect())) {
                    b.Hit();
                    e.GotHit();

                    // create Explosion in enemy position
                    CreateExplosion(e, explosions);
                }
            }
        }
    }
}

void Game::BulletsHitBullets(std::forward_list<Bullet> &player_bullets, 
                            std::forward_list<Bullet> &enemy_bullets) {
    for (auto &pb : player_bullets) {
        if (!pb.IsHit()) {
            for (auto &eb : enemy_bullets) {
                if (!eb.IsHit() && CheckCollision(pb.GetRect(), eb.GetRect())) 
                {
                    pb.Hit();
                    eb.Hit();
                }
            }
        }
    }
}

void Game::BulletsHitPlayer(std::forward_list<Bullet> &bullets, 
                            Fighter &player) {
    if (!player.IsDead()) {
        for (auto &b : bullets) {
            if (!b.IsHit() && CheckCollision(b.GetRect(), player.GetRect())) {
                b.Hit();
                player.GotHit();
                break;
            }
        }
    }
}

void Game::EnemiesHitPlayer(std::forward_list<Fighter> &enemies, 
                            Fighter &player) {
    if (!player.IsDead()) {
        for (auto &e : enemies) {
            if (!e.IsDead() && CheckCollision(e.GetRect(), player.GetRect())) {
                e.GotHit();
                player.GotHit();
                break;
            }
        }
    }
}

void Game::ClearInvalidObjects(std::forward_list<Fighter> &enemies,
                               std::forward_list<Bullet> &player_bullets,
                               std::forward_list<Bullet> &enemy_bullets) {
    SDL_Rect bound_rect = renderer.GetScreenRect();

    // Check whether the enemy got hit or flied out the screen.
    // If yes, remove it
    enemies.remove_if(
    [](Fighter &e) {
        SDL_Rect e_rect = e.GetRect();
        return e.IsDead() || ((e_rect.x + e_rect.w) <= 0);
    });

    // Check whether the bullet got hit or flied out the screen.
    // If yes, remove it
    enemy_bullets.remove_if(
    [](Bullet &eb) {
        SDL_Rect eb_rect = eb.GetRect();
        return eb.IsHit() || ((eb_rect.x + eb_rect.w) <= 0);
    });

    // Check whether the bullet got hit or flied out the screen.
    // If yes, remove it
    player_bullets.remove_if(
    [bound_rect](Bullet &pb) {
        SDL_Rect pb_rect = pb.GetRect();
        return pb.IsHit() || (pb_rect.x > bound_rect.w);
    });
}

void Game::CreateExplosion(Fighter const &fighter_got_hit, 
                           std::forward_list<Explosion> &explosions) {
    Explosion e{explosions_texture, EXPLOSION_PIC_COUNT, EXPLOSION_PIC_INTERVAL};
    SDL_Rect fighter_rect = fighter_got_hit.GetRect();
    SDL_Rect explosion_rect = e.GetRect();

    int explosion_x = (fighter_rect.x + fighter_rect.w / 2) - 
                      (explosion_rect.w / EXPLOSION_PIC_COUNT / 2);
    int explosion_y = (fighter_rect.y + fighter_rect.h / 2) - 
                      (explosion_rect.h / 2);
    e.SetPosition(explosion_x, explosion_y);

    explosions.push_front(std::move(e));
}
