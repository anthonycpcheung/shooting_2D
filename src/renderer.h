#pragma once

#include "SDL.h"

/*
 This class is responsible for rendering the game windows as well as all the 
 objects in the game therein.
 */
class Renderer {
    public:
    // Constructor takes the required screen dimemsion and constructs the 
    // game window.
    Renderer(const std::size_t screen_width, const std::size_t screen_height);
    ~Renderer();

    // Call to render objects of the game
    void Render();

    // Call to update game status to Title bar
    //void UpdateWindowTitle();

    private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    const std::size_t screen_width;
    const std::size_t screen_height;
};