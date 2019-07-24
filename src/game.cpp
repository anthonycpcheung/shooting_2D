#include "game.h"

Game::Game(Renderer &renderer, Controller const &controller) 
    : renderer{renderer}, controller{controller} 
{
    sprites.emplace_back(renderer.CreateSprite("../gfx/player.png"));
}

void Game::Run(std::size_t target_frame_duration) {
    bool running = true;
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    Uint32 title_timestamp = SDL_GetTicks();
    int frame_count = 0;

    int x{100}, y{100};
    sprites[0].SetPosition(x, y);

    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    while(running) {
        frame_start = SDL_GetTicks();

        controller.HandleInput(running, sprites[0]);

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