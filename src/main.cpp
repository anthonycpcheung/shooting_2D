#include <iostream>

#include "controller.h"
#include "renderer.h"

int main(int argc, char *argv[]) {
    constexpr std::size_t screenWidth{1280};
    constexpr std::size_t screenHeight{720};

    Renderer renderer(screenWidth, screenHeight);
    Controller controller;

    bool quit;
    while(!quit) {
        controller.HandleInput(quit);
        renderer.Render();
        SDL_Delay(1000);
    }
    
    return 0;
}