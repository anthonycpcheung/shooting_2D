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

    // Update game status to Title bar
    void UpdateWindowTitle(int fps);

    // Render objects of the game
    void Render(std::vector<Sprite> const &sprites);

    // Create a sprite from an image file
    SDL_Texture *LoadImage(std::string image_filename);

    // Get screen size
    void GetScreenSize(std::size_t &width, std::size_t &height) const;

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