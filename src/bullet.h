#pragma once

#include "SDL.h"
#include "sprite.h"

class Bullet : public Sprite {
    public:
    Bullet(SDL_Texture *sprite_texture, int speed);

    void Hit();
    bool IsHit() const;

    private:
    bool hit;
};
