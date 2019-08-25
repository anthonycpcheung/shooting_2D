#include <algorithm>
#include <iostream>

#include "renderer.h"
#include "SDL_image.h"

Renderer::Renderer(int const screen_width, 
                   int const screen_height,
                   std::string const &title) 
    : screen_width{screen_width}, 
      screen_height{screen_height},
      game_title{title} {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL cound not initialize." << std::endl;
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_image
    int flags=IMG_INIT_JPG|IMG_INIT_PNG;
    int initted=IMG_Init(flags);
    if ((initted&flags) != flags) {
        std::cerr << "SDL_image couuld not initialize." << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
    }
    sdl_image_ready = true;

    // Create Window
    // Window is located on the center of the screen.
    // Use SDL_WINDOWPOS_UNDEFINED to let the OS position the window wherever 
    // it likes.
    sdl_window = SDL_CreateWindow(game_title.c_str(), SDL_WINDOWPOS_CENTERED,
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
    // Destroy window
    SDL_DestroyWindow(sdl_window);

    // clean up SDL_image
    IMG_Quit();

    // clean up SDL
    SDL_Quit();
}

void Renderer::UpdateWindowTitle(int fps) {
    std::string title{game_title + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::BeginRender() {
    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);
}

void Renderer::EndRender() {
    // Update Screen
    SDL_RenderPresent(sdl_renderer);    
}

SDL_Texture *Renderer::LoadImage(std::string image_filename) {
    SDL_Texture *texture;
    texture = IMG_LoadTexture(sdl_renderer, image_filename.c_str());
    return texture;
}

void Renderer::RenderSprite(SDL_Texture *texture, SDL_Rect const &dest_rect) {
    SDL_RenderCopy(sdl_renderer, texture, NULL, &dest_rect);
}

void Renderer::RenderSprite(SDL_Texture *texture, SDL_Rect const &src_rect, 
                            SDL_Rect const &dest_rect) {
    SDL_RenderCopy(sdl_renderer, texture, &src_rect, &dest_rect);
}

SDL_Rect Renderer::GetScreenRect() const {
    return SDL_Rect{ 0, 0, screen_width, screen_height };
}