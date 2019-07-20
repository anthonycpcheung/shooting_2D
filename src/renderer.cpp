#include <iostream>

#include "renderer.h"

Renderer::Renderer(const std::size_t screen_width, 
                   const std::size_t screen_height) 
    : screen_width{screen_width}, screen_height{screen_height} {
    const int rendererFlags{SDL_RENDERER_ACCELERATED};

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL cound not initialize." << std::endl;
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Create Window
    // Window is located on the center of the screen.
    // Use SDL_WINDOWPOS_UNDEFINED to let the OS position the window wherever 
    // it likes.
    sdl_window = SDL_CreateWindow("2D Shooting", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);
    if (sdl_window == nullptr) {
        std::cerr << "Window could not be created." << std::endl;
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Create renderer
    // The -1 tells SDL to use the first graphics acceleration device it finds.
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl_renderer == nullptr) {
        std::cerr << "Renderer could not be created." << std::endl;
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
    }
}

Renderer::~Renderer() {
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}
