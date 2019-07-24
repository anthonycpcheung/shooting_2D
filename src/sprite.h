#pragma once

#include <string>

#include "SDL.h"
#include "SDL_image.h"

class Sprite {
    public:
    Sprite(SDL_Texture *sprite_texture);
    Sprite(SDL_Texture *sprite_texture, int initial_x, int initial_y);
    //~Sprite();

    // For used by Renderer to render the sprite
    SDL_Rect GetRect() const;
    SDL_Texture *GetTexture() const;

    void MoveTo(int new_x, int new_y);

    private:
    SDL_Texture *texture;
    int x, y, w, h;
};