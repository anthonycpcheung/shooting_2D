#include <iostream>

#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char *argv[]) {
    constexpr std::size_t screenWidth{1280};
    constexpr std::size_t screenHeight{720};

    Renderer renderer(screenWidth, screenHeight);
    Controller controller;
    Game game(renderer, controller);
    game.Run();
    
    std::cout << "Game has terminated successfully!" << std::endl;
    return 0;
}