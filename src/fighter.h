#pragma once

#include "SDL.h"
#include "sprite.h"

class Fighter : public Sprite {
    public:
    Fighter(SDL_Texture *sprite_texture, int speed, int life);
    Fighter(SDL_Texture *sprite_texture, int speed, int life, int reload);

    bool Fire(bool triggering);
    void Reload(int count);
    void GotHit();
    bool IsDead();

    private:
    int reload_count;
    int life_count;
};

