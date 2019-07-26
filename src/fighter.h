#pragma once

#include "SDL.h"
#include "sprite.h"

class Fighter : public Sprite {
    public:
    Fighter(SDL_Texture *sprite_texture, int speed, int reload);
    Fighter(SDL_Texture *sprite_texture, int speed, 
            int initial_x, int initial_y, int reload);

    bool CheckReload();
    void Fired();

    private:
    int const reload;
    int reloadCounter;
};

