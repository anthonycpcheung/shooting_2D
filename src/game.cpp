#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>

#include "defs.h"
#include "game.h"

Game::Game(Renderer &renderer, Controller &controller) 
    : renderer{renderer}, controller{controller}
{}

void Game::Run(std::size_t target_frame_duration) {
    bool running = true;
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
    background = std::make_unique<Background>(background_texture, 
                                              BG_SCROLLING_SPEED, 
                                              renderer.GetScreenRect());

    // Initialize game title
    game_title = std::make_unique<GameObject>(title_texture);

    // Initialize game message
    game_message = std::make_unique<GameText>(fonts_texture, FONT_WIDTH);

    // Initially, show the game title
    game_over = true;
    SetShowTtile();

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        auto actions = controller.ProcessEvent();
        running = !actions.QUIT;

        if (!game_over) {
            // Scroll the background
            background->Scroll();

            // Game objects update
            UpdatePlayerObjects(actions);
            UpdateEnemyObjects(eng);
            UpdateExplosions();
            BulletsHitEnemies();
            BulletsHitBullets();
            BulletsHitPlayer();
            EnemiesHitPlayer();
            ClearInvalidObjects();
            UpdateScoreText();

            // If the player is killed, the game is over.
            // Mark the game_over flag to make the game go back to title.
            game_over = player->IsDead();

            // Screen Update
            RenderGamePlayScreen();
        }
        else if (!explosions.empty()) {
            UpdateExplosions();
            ClearInvalidObjects();
            
            // Screen Update
            RenderGamePlayScreen();

            // If the game is over, it will not go back to title
            // immediately, it will wait for all explosions disappears.
            if (explosions.empty()) {                
                // Set back to Title screen
                SetShowTtile();
            }
        }
        else {
            // When user press the FIRE, the game will start
            if (actions.FIRE) {
                // Initialize game
                ResetGame();

                // Screen Update
                RenderGamePlayScreen();
            }
            else {
                // Scroll the background
                background->Scroll();

                // Render Title screen
                RenderGameTitleScreen();
            }
        }

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

void Game::SetShowTtile() {
    auto screen_rect = renderer.GetScreenRect();

    // Set Title
    auto title_rect = game_title->GetRect();
    game_title->SetPosition((screen_rect.w / 2) - (title_rect.w / 2),
                            150);

    // Set message
    game_message->SetText("PRESS FIRE (LEFT CTRL) TO START.");
    auto message_rect = game_message->GetRect();
    game_message->SetPosition((screen_rect.w / 2) - (message_rect.w / 2),
                               screen_rect.h * 2 / 3);
}

void Game::ResetGame() {
    auto screen_rect = renderer.GetScreenRect();

    // Reset score
    score = 0;
    game_message->SetPosition(10, 10);
    UpdateScoreText();

    // Reset enemy swpan timer;
    enemySpwanTimer = MAX_SPWAN_TIME;

    // Initialize a new player
    player.reset();
    player = std::make_unique<Fighter>(player_texture, 
                                       PLAYER_SPEED, 
                                       PLAYER_LIFE);
    player->SetPosition(100, (screen_rect.h / 2) - (player->GetRect().h / 2));

    // Clear enemies, bullets, explosions
    player_bullets.clear();
    enemies.clear();
    enemy_bullets.clear();
    explosions.clear();

    game_over = false;
}

void Game::RenderGameTitleScreen() {
    renderer.BeginRender();

    // Render Background
    background->Render(renderer);

    // Render Game Title
    game_title->Render(renderer);

    // Render press fire message
    game_message->Render(renderer);

    renderer.EndRender();
}

void Game::RenderGamePlayScreen() {
    renderer.BeginRender();

    // Render Background
    background->Render(renderer);

    // Render player
    if (!player->IsDead()) {
        player->Render(renderer);
    }

    // Render player bullets
    for (auto &bullet : player_bullets) {
        bullet->Render(renderer);
    }

    // Render enemies
    for (auto &enemy : enemies) {
        enemy->Render(renderer);
    }

    // Render enemy bullets
    for (auto &e_bullet : enemy_bullets) {
        e_bullet->Render(renderer);
    }

    // Render explosions
    for (auto &explosion : explosions) {
        explosion->Render(renderer);
    }

    // Render score
    game_message->Render(renderer);

    renderer.EndRender();
}

void Game::UpdatePlayerObjects(Controller::Actions const &actions) {
    auto bound_rect = renderer.GetScreenRect();

    // Move player
    player->Move(actions.UP, actions.DOWN, actions.LEFT, actions.RIGHT);
    player->BoundAdjust(bound_rect);

    // Move flying bullets
    for (auto &b : player_bullets) {
        b->Move(false, false, false, true);
    }

    // Fire new bullet
    if (player->Fire(actions.FIRE)) {
        // The new bullet appears at the right-center of the player
        auto new_bullet = std::make_unique<Bullet>(player_bullet_texture, 
                                                   BULLET_SPEED);
        auto player_rect_new = player->GetRect();
        auto bullet_rect = new_bullet->GetRect();
        int bullet_x = player_rect_new.x + player_rect_new.w;
        int bullet_y = player_rect_new.y + 
                       (player_rect_new.h - bullet_rect.h) / 2;
        new_bullet->SetPosition(bullet_x, bullet_y);
        player_bullets.push_front(std::move(new_bullet));

        // reset the reload counter of player
        player->Reload(PLAYER_RELOAD);
    }
}

void Game::UpdateEnemyObjects(std::mt19937 &eng) {
    auto bound_rect = renderer.GetScreenRect();

    // Move flying enemies
    for (auto &e : enemies) {
        e->Move(false, false, true, false);
    }

    // Move flying bullets
    for (auto &b : enemy_bullets) {
        b->Move(false, false, true, false);
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

        auto new_enemy = std::make_unique<Fighter>(enemy_texture, 
                                                   random_speed(eng), 
                                                   ENEMY_LIFE, 
                                                   random_fire(eng));

        int max_y = bound_rect.h - new_enemy->GetRect().h;

        // The initial y position of new enemy is uniform distributed
        std::uniform_int_distribution<int> random_position(0, max_y);
        int pos_y = random_position(eng);

        if (pos_y > max_y) {
            pos_y = max_y;
        }
        new_enemy->SetPosition(bound_rect.w, random_position(eng));
        enemies.push_front(std::move(new_enemy));

        // reset the enemy spwan count randomly
        enemySpwanTimer = random_spwan(eng);
    }

    // Fire new bullets
    for (auto &enemy : enemies) {
        auto enemy_rect = enemy->GetRect();

        if (enemy->Fire(true)) {
            // The new bullet appears at the left-center of the enemy
            auto new_bullet = std::make_unique<Bullet>(enemy_bullet_texture, 
                                                       BULLET_SPEED);
            auto bullet_rect = new_bullet->GetRect();
            int bullet_x = enemy_rect.x - bullet_rect.w;
            int bullet_y = enemy_rect.y + (enemy_rect.h - bullet_rect.h) / 2;
            new_bullet->SetPosition(bullet_x, bullet_y);
            enemy_bullets.push_front(std::move(new_bullet));
            enemy->Reload(random_fire(eng));
        }        
    }
}

void Game::UpdateExplosions() {
    for (auto & explosion : explosions) {
        // update the counter of the explosion
        explosion->UpdateCounter();
    }

    // remove the explosion if it is disappeared
    explosions.remove_if([](std::unique_ptr<Explosion> &e) {
        return e->Disappeared();
    });
}

void Game::PreloadTextures() {
    player_texture = renderer.LoadImage(PLAYER_IMAGE);
    player_bullet_texture = renderer.LoadImage(PLAYER_BULLET_IMAGE);
    enemy_texture = renderer.LoadImage(ENEMY_IMAGE);
    enemy_bullet_texture = renderer.LoadImage(ENEMY_BULLET_IMAGE);
    background_texture = renderer.LoadImage(BACKGROUND_IMAGE);
    explosions_texture = renderer.LoadImage(EXPLOSIONS_IMAGE);
    fonts_texture = renderer.LoadImage(FONTS_IMAGE);
    title_texture = renderer.LoadImage(TITLE_IMAGE);
}

bool Game::CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2) {
    // check whether the rectangle of the two objects are intercepted
    return (std::max(rect1.x, rect2.x) 
                < std::min(rect1.x + rect1.w, rect2.x + rect2.w)) && 
           (std::max(rect1.y, rect2.y) 
                < std::min(rect1.y + rect1.h, rect2.y + rect2.h));
}

void Game::BulletsHitEnemies() {
    for (auto &b : player_bullets) {
        if (!b->IsHit()) {
            for (auto &e : enemies) {
                if (!e->IsDead() && CheckCollision(b->GetRect(), e->GetRect())) {
                    // player bullet hits an enemy
                    // update the states of bullet and enemy
                    b->Hit();
                    e->GotHit();

                    // create Explosion at enemy position
                    CreateExplosion(*e);

                    // increase score;
                    ++score;

                    break;
                }
            }
        }
    }
}

void Game::BulletsHitBullets() {
    for (auto &pb : player_bullets) {
        if (!pb->IsHit()) {
            for (auto &eb : enemy_bullets) {
                if (!eb->IsHit() && CheckCollision(pb->GetRect(), eb->GetRect())) 
                {
                    // player bullet and enemy bullet collide
                    // update the states of bullets
                    pb->Hit();
                    eb->Hit();

                    break;
                }
            }
        }
    }
}

void Game::BulletsHitPlayer() {
    if (!player->IsDead()) {
        for (auto &b : enemy_bullets) {
            if (!b->IsHit() && CheckCollision(b->GetRect(), player->GetRect())) {
                // enemy bullet hits player
                // update the states of bullet and player
                b->Hit();
                player->GotHit();

                // create Explosion at player position
                CreateExplosion(*player);

                break;
            }
        }
    }
}

void Game::EnemiesHitPlayer() {
    if (!player->IsDead()) {
        for (auto &e : enemies) {
            if (!e->IsDead() && CheckCollision(e->GetRect(), player->GetRect())) {
                // player and enemy collide
                // update the states of player and enemy
                e->GotHit();
                player->GotHit();

                // create Explosions at player and enemy positions
                CreateExplosion(*e);
                CreateExplosion(*player);

                break;
            }
        }
    }
}

void Game::ClearInvalidObjects() {
    auto bound_rect = renderer.GetScreenRect();

    // Check whether the enemy got hit or flied out the screen.
    // If yes, remove it
    enemies.remove_if(
    [](std::unique_ptr<Fighter> &e) {
        auto e_rect = e->GetRect();
        return e->IsDead() || ((e_rect.x + e_rect.w) <= 0);
    });

    // Check whether the enemy bullet got hit or flied out the screen.
    // If yes, remove it
    enemy_bullets.remove_if(
    [](std::unique_ptr<Bullet> &eb) {
        auto eb_rect = eb->GetRect();
        return eb->IsHit() || ((eb_rect.x + eb_rect.w) <= 0);
    });

    // Check whether the player bullet got hit or flied out the screen.
    // If yes, remove it
    player_bullets.remove_if(
    [bound_rect](std::unique_ptr<Bullet> &pb) {
        auto pb_rect = pb->GetRect();
        return pb->IsHit() || (pb_rect.x > bound_rect.w);
    });
}

void Game::CreateExplosion(Fighter const &fighter_got_hit) {
    auto ex = std::make_unique<Explosion>(explosions_texture, 
                                          EXPLOSION_PIC_COUNT, 
                                          EXPLOSION_PIC_INTERVAL);
    auto fighter_rect = fighter_got_hit.GetRect();
    auto explosion_rect = ex->GetRect();

    // Position the explosion at the center of the fighter
    int explosion_x = (fighter_rect.x + fighter_rect.w / 2) - 
                      (explosion_rect.w / 2);
    int explosion_y = (fighter_rect.y + fighter_rect.h / 2) - 
                      (explosion_rect.h / 2);
    ex->SetPosition(explosion_x, explosion_y);

    explosions.push_front(std::move(ex));
}

void Game::UpdateScoreText() {
    std::ostringstream ss;
    ss << "SCORE: " << score;
    game_message->SetText(ss.str());
}