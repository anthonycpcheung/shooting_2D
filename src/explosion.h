#pragma once

#include "sprite.h"

class Explosion : public Sprite {
    public:
    Explosion(SDL_Texture *texture, std::size_t pic_count, int pic_interval);
    void Render(Renderer &renderer) const;
    SDL_Rect GetRect() const;
    void UpdateCounter();
    bool Disappeared();

    private:
    std::size_t pic_count;
    int pic_interval;
    int counter;
};