#pragma once

#include <string>
#include <vector>

#include "SDL.h"
#include "sprite.h"

/*
 This class is responsible for rendering the game windows as well as all the 
 objects in the game therein.
 */
class Renderer {
    public:
    // Constructor takes the required screen dimemsion and constructs the 
    // game window.
    Renderer(std::size_t const screen_width, std::size_t const screen_height, 
             std::string const &title);
    ~Renderer();

    // Call to update game status to Title bar
    void UpdateWindowTitle(int fps);

    // Call to render objects of the game
    void Render(std::vector<Sprite> const &sprites);

    // Call to create a sprite from an image file
    Sprite CreateSprite(std::string sprite_image_filename);

    private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;

    std::size_t const screen_width;
    std::size_t const screen_height;
    std::string const game_title;

    bool sdl_image_ready;

    // Call to render a sprite
    void RenderSprite(Sprite const &sprite);
};