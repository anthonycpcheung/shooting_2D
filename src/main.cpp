#include <iostream>

#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char *argv[]) {
    constexpr std::size_t screenWidth{1280};
    constexpr std::size_t screenHeight{720};
    constexpr std::size_t framePerSecond{60};
    constexpr std::size_t msPerFrame{1000 / framePerSecond};

    Renderer renderer(screenWidth, screenHeight, "2D Shooting");
    Controller controller;
    Game game(renderer, controller);
    game.Run(msPerFrame);
    
    std::cout << "Game has terminated successfully!" << std::endl;
    return 0;
}