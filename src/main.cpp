#include <iostream>

#include "defs.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"

int main(int argc, char *argv[]) {
    Renderer renderer(screenWidth, screenHeight, "2D Shooting");
    Controller controller;
    Game game(renderer, controller);
    game.Run(msPerFrame);
    
    std::cout << "Game has terminated successfully!" << std::endl;
    return 0;
}