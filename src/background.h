#pragma once

#include "sprite.h"

class Background : public Sprite {
    public:
    Background(SDL_Texture *texture, int rollingSpeed, 
               SDL_Rect const &screen_rect);
    void Render(Renderer &renderer) const;
    void Scroll();

    private:
    SDL_Rect screen_rect;
};