#include "game.h"

int constexpr PLAYER_SPEED{5};
int constexpr BULLET_SPEED{8};
Sprite::MoveDir constexpr PLAYER_BULLET_DIR{Sprite::MoveDir::RIGHT};

Game::Game(Renderer &renderer, Controller const &controller) 
    : renderer{renderer}, controller{controller} 
{
}

void Game::Run(std::size_t target_frame_duration) {
    bool running = true;
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    Uint32 title_timestamp = SDL_GetTicks();
    int frame_count = 0;

    sprites.emplace_back(CreateSprite("../gfx/player.png", PLAYER_SPEED));
    sprites.emplace_back(CreateSprite("../gfx/playerBullet.png", BULLET_SPEED));
    auto &player = sprites[0];
    auto &playerBullet = sprites[1];

    int x{100}, y{100};
    player.SetPosition(x, y);
    player.SetShow(true);

    Controller::Actions playerActions;

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        controller.ProcessEvent(running, playerActions);

        UpdatePlayerObjects(playerActions);
        UpdateNonplayerObjects();

        renderer.Render(sprites);
        
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

void Game::UpdatePlayerObjects(Controller::Actions const &actions) {
    Sprite &player = sprites[0];
    Sprite &player_bullet = sprites[1];

    std::size_t width_bound, height_bound;
    renderer.GetScreenSize(width_bound, height_bound);

    if (actions.UP) {
        player.Move(Sprite::MoveDir::UP);
    }

    if (actions.DOWN) {
        player.Move(Sprite::MoveDir::DOWN);
    }

    if (actions.LEFT) {
        player.Move(Sprite::MoveDir::LEFT);
    }

    if (actions.RIGHT) {
        player.Move(Sprite::MoveDir::RIGHT);
    }

    bool new_bullet = false;
    if (actions.FIRE) {
        if (!player_bullet.GetShow()) {
            // bullet emitted from player
            SDL_Rect player_rect = player.GetRect();
            SDL_Rect bullet_rect = player_bullet.GetRect();
            int bullet_x = player_rect.x + player_rect.w;
            int bullet_y = player_rect.y + (player_rect.h - bullet_rect.h) / 2;
            player_bullet.SetPosition(bullet_x, bullet_y);
            player_bullet.SetShow(true);
            new_bullet = true;
        }
    }

    if (player_bullet.GetShow() && !new_bullet) {
        // bullet is flying
        player_bullet.Move(PLAYER_BULLET_DIR);

        // Check whether the bullet flies out the screen.
        // If yes, set it as not showing
        SDL_Rect bullet_rect = player_bullet.GetRect();
        if (bullet_rect.x > width_bound) {
            player_bullet.SetShow(false);
        }
    }
}

void Game::UpdateNonplayerObjects() {

}

Sprite Game::CreateSprite(std::string sprite_image_filename, int speed) 
{
    SDL_Texture *texture;
    texture = renderer.LoadImage(sprite_image_filename.c_str());
    return Sprite(texture, speed);
}
