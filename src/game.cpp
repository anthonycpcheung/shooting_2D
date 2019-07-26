#include <algorithm>
#include <iostream>

#include "game.h"

int constexpr PLAYER_SPEED{4};
int constexpr BULLET_SPEED{8};
int constexpr BULLET_RELOAD{8};
MoveDir constexpr PLAYER_BULLET_DIR{MoveDir::RIGHT};

std::string const TEXTURE_BASE{"../gfx/"};
std::string const PLAYER_TAG{"player.png"};
std::string const PLAYER_BULLET_TAG{"playerBullet.png"};

Game::Game(Renderer &renderer, Controller &controller) 
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

    // Pre-load game objects images
    std::unordered_map<std::string, SDL_Texture *> textures;
    textures[PLAYER_TAG] = renderer.LoadImage(TEXTURE_BASE + PLAYER_TAG);
    textures[PLAYER_BULLET_TAG] = renderer.LoadImage(TEXTURE_BASE + PLAYER_BULLET_TAG);

    // Initialize player
    Fighter player{renderer.LoadImage(TEXTURE_BASE + PLAYER_TAG), 
                   PLAYER_SPEED, BULLET_RELOAD};
    std::vector<Bullet> player_bullets;

    player.SetPosition(100, 100);

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        auto actions = controller.ProcessEvent();
        running = !actions.QUIT;

        UpdatePlayerObjects(actions, player, player_bullets);
        UpdateNonplayerObjects();

        renderer.BeginRender();

        // Render player
        renderer.RenderSprite(player);

        // Render player bullets
        for (auto &bullet : player_bullets) {
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
        b.Move(PLAYER_BULLET_DIR);
    }

    // Fire new bullet
    if (player.CheckReload() && actions.FIRE) {
        Bullet new_bullet{renderer.LoadImage(TEXTURE_BASE + PLAYER_BULLET_TAG), 
                          BULLET_SPEED};
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

void Game::UpdateNonplayerObjects() {

}
