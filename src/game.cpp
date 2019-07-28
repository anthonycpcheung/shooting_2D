#include <algorithm>
#include <iostream>
#include <random>

#include "game.h"

Game::Game(Renderer &renderer, Controller &controller) 
    : renderer{renderer}, controller{controller}, enemySpwanTimer{90} 
{
}

void Game::Run(std::size_t target_frame_duration) {
    int constexpr PLAYER_SPEED{4};

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
    std::vector<Bullet> player_bullets;

    // Initialize enemies
    std::vector<Fighter> enemies;
    std::vector<Bullet> enemy_bullets;

    player.SetPosition(100, 100);

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        auto actions = controller.ProcessEvent();
        running = !actions.QUIT;

        UpdatePlayerObjects(actions, player, player_bullets);
        UpdateNonplayerObjects(enemies, enemy_bullets, eng);

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
        for (auto &bullet : enemy_bullets) {
            renderer.RenderSprite(bullet);
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
                               std::vector<Bullet> &bullets) {
    int constexpr BULLET_SPEED{8};

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
    if (player.CheckReload() && actions.FIRE) {
        Bullet new_bullet{player_bullet_texture, BULLET_SPEED};
        SDL_Rect player_rect_new = player.GetRect();
        SDL_Rect bullet_rect = new_bullet.GetRect();
        int bullet_x = player_rect_new.x + player_rect_new.w;
        int bullet_y = player_rect_new.y + (player_rect_new.h - bullet_rect.h) / 2;
        new_bullet.SetPosition(bullet_x, bullet_y);
        bullets.push_back(std::move(new_bullet));
        player.Fired();
    }

    // Check whether the bullet flies out the screen.
    // If yes, remove it
    std::remove_if(bullets.begin(), bullets.end(),
    [width_bound](Bullet const &b) {
        SDL_Rect b_rect = b.GetRect();
        return (b_rect.x > width_bound);
    });
}

void Game::UpdateNonplayerObjects(std::vector<Fighter> &enemies, 
                                  std::vector<Bullet> &bullets,
                                  std::mt19937 &eng) {
    int constexpr MIN_SPWAN_TIME{30};
    int constexpr MAX_SPWAN_TIME{90};
    int constexpr MIN_ENEMY_SPEED{2};
    int constexpr MAX_ENEMY_SPEED{6};

    std::size_t width_bound, height_bound;
    renderer.GetScreenSize(width_bound, height_bound);

    // Move flying enemies
    for (auto &e : enemies) {
        e.Move(MoveDir::LEFT);
    }

    // Check whether the enemy flies out the screen.
    // If yes, remove it
    std::remove_if(enemies.begin(), enemies.end(),
    [](Fighter const &e) {
        SDL_Rect e_rect = e.GetRect();
        return ((e_rect.x + e_rect.w) <= 0);
    });

    // Move flying bullets
    for (auto &b : bullets) {
        b.Move(MoveDir::LEFT);
    }

    // Check whether the bullet flies out the screen.
    // If yes, remove it
    std::remove_if(bullets.begin(), bullets.end(),
    [](Bullet const &b) {
        SDL_Rect b_rect = b.GetRect();
        return ((b_rect.x + b_rect.w) <= 0);
    });

    // Spwan new enemy
    if (--enemySpwanTimer <= 0) {
        std::uniform_int_distribution<int> random_spwan(MIN_SPWAN_TIME, 
                                                        MAX_SPWAN_TIME);
        std::uniform_int_distribution<int> random_speed(MIN_ENEMY_SPEED, 
                                                        MAX_ENEMY_SPEED);

        Fighter new_enemy{enemy_texture, random_speed(eng)};

        int max_y = height_bound - new_enemy.GetRect().h;

        std::uniform_int_distribution<int> random_position(0, max_y);
        int pos_y = random_position(eng);

        if (pos_y > max_y) {
            pos_y = max_y;
        }
        new_enemy.SetPosition(width_bound, random_position(eng));
        enemies.push_back(std::move(new_enemy));

        enemySpwanTimer = random_spwan(eng);
    }
}

void Game::PreloadTextures() {
    std::string const PLAYER_IMAGE{"../gfx/player.png"};
    std::string const PLAYER_BULLET_IMAGE{"../gfx/playerBullet.png"};
    std::string const ENEMY_IMAGE("../gfx/enemy.png");

    player_texture = renderer.LoadImage(PLAYER_IMAGE);
    player_bullet_texture = renderer.LoadImage(PLAYER_BULLET_IMAGE);
    enemy_texture = renderer.LoadImage(ENEMY_IMAGE);
}

bool Game::CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2) {
    return (std::max(rect1.x, rect2.x) 
                < std::min(rect1.x + rect1.w, rect2.x + rect2.w)) && 
           (std::max(rect1.y, rect2.y) 
                < std::min(rect1.y + rect1.h, rect2.y + rect2.h));
}