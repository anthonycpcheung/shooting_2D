#pragma once

#include "SDL.h"
#include "gameobject.h"

class Fighter : public GameObject {
    public:
    Fighter(SDL_Texture *texture, int speed, int life);
    Fighter(SDL_Texture *texture, int speed, int life, int reload);

    // Return true if the fighter can fire a bullet
    bool Fire(bool triggering);

    // Reset the reload count for firing bullet
    void Reload(int count);

    // Deduct the life of fighter 
    void GotHit();

    // Return true if the life count of the fighter become zero
    bool IsDead();

    private:
    int reload_count;
    int life_count;
};

