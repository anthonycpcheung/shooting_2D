#pragma once

#include "SDL.h"
#include "gameobject.h"

class Bullet : public GameObject {
    public:
    Bullet(SDL_Texture *texture, int speed);

    // Set that the bullet hits something
    void Hit();

    // Return true if the bullet hits something
    bool IsHit() const;

    private:
    bool hit;
};
