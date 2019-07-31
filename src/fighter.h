#pragma once

#include "SDL.h"
#include "sprite.h"

class Fighter : public Sprite {
    public:
    Fighter(SDL_Texture *sprite_texture, int speed);
    Fighter(SDL_Texture *sprite_texture, int speed, int bullets);

    bool Fire(bool triggering);

    private:
    int reload_counter;
    int bullet_count;
};

