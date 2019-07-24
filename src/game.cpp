#include "game.h"

Game::Game(Renderer &renderer, Controller const &controller) 
    : renderer{renderer}, controller{controller} 
{
    sprites.emplace_back(renderer.CreateSprite("../gfx/player.png"));
}

void Game::Run() {
    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    bool running = true;

    int x{100}, y{100};
    while(running) {
        controller.HandleInput(running);

        sprites[0].MoveTo(x, y);
        x++;
        y++;

        renderer.Render(sprites);
        SDL_Delay(1000);
    }
}