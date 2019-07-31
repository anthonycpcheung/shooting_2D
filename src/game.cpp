#include <algorithm>
#include <iostream>
#include <random>

#include "game.h"

int constexpr PLAYER_SPEED{4};
int constexpr BULLET_SPEED{8};
int constexpr ENEMY_BULLETS{4};
int constexpr MIN_SPWAN_TIME{30};
int constexpr MAX_SPWAN_TIME{90};
int constexpr MIN_ENEMY_SPEED{2};
int constexpr MAX_ENEMY_SPEED{6};

Game::Game(Renderer &renderer, Controller &controller) 
    : renderer{renderer}, controller{controller}, enemySpwanTimer{90} 
{
}

void Game::Run(std::size_t target_frame_duration) {
    bool running = true;
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    Uint32 title_timestamp = SDL_GetTicks();
    int frame_count = 0;

    // Random generator for enemy spwan, enemy speed, and enemy position
    std::random_device rd;
    std::mt19937 eng(rd());

    // Pre-load game objects images
    PreloadTextures();

    // Initialize player
    Fighter player{player_texture, PLAYER_SPEED};
    std::forward_list<Bullet> player_bullets;

    // Initialize enemies
    std::forward_list<Fighter> enemies;
    std::forward_list<Bullet> enemy_bullets;

    player.SetPosition(100, 100);

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        auto actions = controller.ProcessEvent();
        running = !actions.QUIT;

        if (!player.GetHit()) {
            UpdatePlayerObjects(actions, player, player_bullets);
            UpdateNonplayerObjects(enemies, enemy_bullets, eng);
            BulletsHitEnemies(player_bullets, enemies);
            BulletsHitBullets(player_bullets, enemy_bullets);
            BulletsHitPlayer(enemy_bullets, player);
            EnemiesHitPlayer(enemies, player);
            ClearInvalidObjects(enemies, player_bullets, enemy_bullets);
        }

        renderer.BeginRender();

        // Render player
        renderer.RenderSprite(player);

        // Render player bullets
        for (auto &bullet : player_bullets) {
            renderer.RenderSprite(bullet);
        }

        // Render enemies
        for (auto &enemy : enemies) {
            renderer.RenderSprite(enemy);
        }

        // Render enemy bullets
        for (auto &e_bullet : enemy_bullets) {
            renderer.RenderSprite(e_bullet);
        }

        renderer.EndRender();
        
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

void Game::UpdatePlayerObjects(Controller::Actions const &actions, 
                               Fighter &player, 
                               std::forward_list<Bullet> &bullets) {
    std::size_t width_bound, height_bound;
    renderer.GetScreenSize(width_bound, height_bound);

    // Move player
    if (actions.UP) {
        player.MoveWithBoundFix(MoveDir::UP, width_bound, height_bound);
    }

    if (actions.DOWN) {
        player.MoveWithBoundFix(MoveDir::DOWN, width_bound, height_bound);
    }

    if (actions.LEFT) {
        player.MoveWithBoundFix(MoveDir::LEFT, width_bound, height_bound);
    }

    if (actions.RIGHT) {
        player.MoveWithBoundFix(MoveDir::RIGHT, width_bound, height_bound);
    }

    // Move flying bullets
    for (auto &b : bullets) {
        b.Move(MoveDir::RIGHT);
    }

    // Fire new bullet
    if (player.Fire(actions.FIRE)) {
        Bullet new_bullet{player_bullet_texture, BULLET_SPEED};
        SDL_Rect player_rect_new = player.GetRect();
        SDL_Rect bullet_rect = new_bullet.GetRect();
        int bullet_x = player_rect_new.x + player_rect_new.w;
        int bullet_y = player_rect_new.y + (player_rect_new.h - bullet_rect.h) / 2;
        new_bullet.SetPosition(bullet_x, bullet_y);
        bullets.push_front(std::move(new_bullet));
    }
}

void Game::UpdateNonplayerObjects(std::forward_list<Fighter> &enemies, 
                                  std::forward_list<Bullet> &bullets,
                                  std::mt19937 &eng) {
    std::size_t width_bound, height_bound;
    renderer.GetScreenSize(width_bound, height_bound);

    // Move flying enemies
    for (auto &e : enemies) {
        e.Move(MoveDir::LEFT);
    }

    // Move flying bullets
    for (auto &b : bullets) {
        b.Move(MoveDir::LEFT);
    }

    // Spwan new enemy
    if (--enemySpwanTimer <= 0) {
        std::uniform_int_distribution<int> random_spwan(MIN_SPWAN_TIME, 
                                                        MAX_SPWAN_TIME);
        std::uniform_int_distribution<int> random_speed(MIN_ENEMY_SPEED, 
                                                        MAX_ENEMY_SPEED);

        Fighter new_enemy{enemy_texture, random_speed(eng), ENEMY_BULLETS};

        int max_y = height_bound - new_enemy.GetRect().h;

        std::uniform_int_distribution<int> random_position(0, max_y);
        int pos_y = random_position(eng);

        if (pos_y > max_y) {
            pos_y = max_y;
        }
        new_enemy.SetPosition(width_bound, random_position(eng));
        enemies.push_front(std::move(new_enemy));

        enemySpwanTimer = random_spwan(eng);
    }

    // Fire new bullets
    std::uniform_int_distribution<int> random_fire(0, 1);
    for (auto &enemy : enemies) {
        SDL_Rect enemy_rect = enemy.GetRect();
        bool trigger = ((enemy_rect.x > width_bound / 2) || 
                       (enemy_rect.x < width_bound / 6)) &&
                       random_fire(eng);

        if (enemy.Fire(trigger)) {
            Bullet new_bullet{enemy_bullet_texture, BULLET_SPEED};
            SDL_Rect bullet_rect = new_bullet.GetRect();
            int bullet_x = enemy_rect.x - bullet_rect.w;
            int bullet_y = enemy_rect.y + (enemy_rect.h - bullet_rect.h) / 2;
            new_bullet.SetPosition(bullet_x, bullet_y);
            bullets.push_front(std::move(new_bullet));
        }        
    }
}

void Game::PreloadTextures() {
    std::string const PLAYER_IMAGE{"../gfx/player.png"};
    std::string const PLAYER_BULLET_IMAGE{"../gfx/playerBullet.png"};
    std::string const ENEMY_IMAGE("../gfx/enemy.png");
    std::string const ENEMY_BULLET_IMAGE("../gfx/enemyBullet.png");

    player_texture = renderer.LoadImage(PLAYER_IMAGE);
    player_bullet_texture = renderer.LoadImage(PLAYER_BULLET_IMAGE);
    enemy_texture = renderer.LoadImage(ENEMY_IMAGE);
    enemy_bullet_texture = renderer.LoadImage(ENEMY_BULLET_IMAGE);
}

bool Game::CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2) {
    return (std::max(rect1.x, rect2.x) 
                < std::min(rect1.x + rect1.w, rect2.x + rect2.w)) && 
           (std::max(rect1.y, rect2.y) 
                < std::min(rect1.y + rect1.h, rect2.y + rect2.h));
}

void Game::BulletsHitEnemies(std::forward_list<Bullet> &bullets, 
                            std::forward_list<Fighter> &enemies) {
    for (auto &b : bullets) {
        if (!b.GetHit()) {
            for (auto &e : enemies) {
                if (!e.GetHit() && CheckCollision(b.GetRect(), e.GetRect())) {
                    b.SetHit();
                    e.SetHit();
                }
            }
        }
    }
}

void Game::BulletsHitBullets(std::forward_list<Bullet> &player_bullets, 
                            std::forward_list<Bullet> &enemy_bullets) {
    for (auto &pb : player_bullets) {
        if (!pb.GetHit()) {
            for (auto &eb : enemy_bullets) {
                if (!eb.GetHit() && CheckCollision(pb.GetRect(), eb.GetRect())) {
                    pb.SetHit();
                    eb.SetHit();
                }
            }
        }
    }
}

void Game::BulletsHitPlayer(std::forward_list<Bullet> &bullets, 
                            Fighter &player) {
    if (!player.GetHit()) {
        for (auto &b : bullets) {
            if (!b.GetHit() && CheckCollision(b.GetRect(), player.GetRect())) {
                b.SetHit();
                player.SetHit();
                break;
            }
        }
    }
}

void Game::EnemiesHitPlayer(std::forward_list<Fighter> &enemies, 
                            Fighter &player) {
    if (!player.GetHit()) {
        for (auto &e : enemies) {
            if (!e.GetHit() && CheckCollision(e.GetRect(), player.GetRect())) {
                e.SetHit();
                player.SetHit();
                break;
            }
        }
    }
}

void Game::ClearInvalidObjects(std::forward_list<Fighter> &enemies,
                               std::forward_list<Bullet> &player_bullets,
                               std::forward_list<Bullet> &enemy_bullets) {
    std::size_t width_bound, height_bound;
    renderer.GetScreenSize(width_bound, height_bound);

    // Check whether the enemy gets hit or flies out the screen.
    // If yes, remove it
    enemies.remove_if(
    [](Fighter &e) {
        SDL_Rect e_rect = e.GetRect();
        return e.GetHit() || ((e_rect.x + e_rect.w) <= 0);
    });

    // Check whether the bullet get hit or flies out the screen.
    // If yes, remove it
    enemy_bullets.remove_if(
    [](Bullet &eb) {
        SDL_Rect eb_rect = eb.GetRect();
        return eb.GetHit() || ((eb_rect.x + eb_rect.w) <= 0);
    });

    // Check whether the bullet get hit or flies out the screen.
    // If yes, remove it
    player_bullets.remove_if(
    [width_bound](Bullet &pb) {
        SDL_Rect pb_rect = pb.GetRect();
        return pb.GetHit() || (pb_rect.x > width_bound);
    });

}
