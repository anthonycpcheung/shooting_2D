#pragma once

#include "SDL.h"
#include "sprite.h"

class Bullet : public Sprite {
    public:
    Bullet(SDL_Texture *sprite_texture, int speed);
    Bullet(SDL_Texture *sprite_texture, int speed, 
            int initial_x, int initial_y);
};
