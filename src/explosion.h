#pragma once

#include "gameobject.h"

class Explosion : public GameObject {
    public:
    Explosion(SDL_Texture *texture, std::size_t pic_count, int pic_interval);
    void Render(Renderer &renderer) const override;
    SDL_Rect GetRect() const override;

    // Update the counter of the explosion
    void UpdateCounter();

    // Return true if the explosion is ended
    bool Disappeared();

    private:
    std::size_t pic_count;
    int pic_interval;
    int counter;
};