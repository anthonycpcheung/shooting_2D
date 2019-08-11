#include "background.h"

Background::Background(SDL_Texture *texture, int rollingSpeed, 
                       SDL_Rect const &screen_rect) 
    : Sprite{texture, rollingSpeed}, screen_rect{screen_rect}
{}

void Background::Render(Renderer &renderer) const {
    for (int dest_x = x; dest_x < screen_rect.w; dest_x += screen_rect.w) {
        SDL_Rect dest{dest_x, 0, screen_rect.w, screen_rect.h};
        renderer.RenderSprite(texture, dest);
    }
}

void Background::Scroll() {
    x -= speed;
    if (x < -screen_rect.w) {
        x = 0;
    }
}