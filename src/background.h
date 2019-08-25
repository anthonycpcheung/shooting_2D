#pragma once

#include "gameobject.h"

class Background : public GameObject {
    public:
    Background(SDL_Texture *texture, int rollingSpeed, 
               SDL_Rect const &screen_rect);
    void Render(Renderer &renderer) const override;

    // Scroll the background according to defined speed
    void Scroll();

    private:
    SDL_Rect screen_rect;
};