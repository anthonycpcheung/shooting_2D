#pragma once

#include "sprite.h"

class Explosion : public Sprite {
    public:
    Explosion(SDL_Texture *texture, int pic_count, int pic_interval);
    void Render(Renderer &renderer) const;
    void UpdateCounter();
    bool Disappeared();

    private:
    int pic_count;
    int pic_interval;
    int counter;
};