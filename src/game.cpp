#include "game.h"

Game::Game(Renderer &renderer, Controller const &controller) 
    : renderer{renderer}, controller{controller} 
    {}

void Game::Run() {
    // The main game loop starts here
    // Each loop goes through Input, Update, and Render phases
    bool running = true;

    while(running) {
        controller.HandleInput(running);
        renderer.Render();
        SDL_Delay(1000);
    }
}