#pragma once

#include <string>
#include <vector>

#include "SDL.h"

/*
 This class is responsible for rendering the game windows as well as all the 
 objects in the game therein.
 */
class Renderer {
    public:
    // Constructor takes the required screen dimemsion and constructs the 
    // game window.
    Renderer(int const screen_width, int const screen_height, 
             std::string const &title);
    ~Renderer();

    // Update game status to Title bar
    void UpdateWindowTitle(int fps);

    // Enclose the rendering of objects of the game
    void BeginRender();
    void EndRender();

    // Render the whole SDL texture
    void RenderSprite(SDL_Texture *texture, SDL_Rect const &dest_rect);

    // Render a specific area of SDL texture
    void RenderSprite(SDL_Texture *texture, SDL_Rect const &src_rect, 
                      SDL_Rect const &dest_rect);

    // Load an image file as SDL texture
    SDL_Texture *LoadImage(std::string image_filename);

    // Get screen dimension
    SDL_Rect GetScreenRect() const;

    private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    int const screen_width;
    int const screen_height;
    std::string const game_title;

    bool sdl_image_ready;
};