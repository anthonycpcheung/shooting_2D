#include "explosion.h"

Explosion::Explosion(SDL_Texture *texture, int pic_count, int pic_interval)
    : Sprite{texture, 0}, 
    pic_count{pic_count}, pic_interval{pic_interval}, 
    counter{0}
{}

void Explosion::Render(Renderer &renderer) const {
    int pic_width = w / pic_count;
    int pic_num = counter / pic_count;
    SDL_Rect src_rect {(pic_num * pic_width),
                       0,
                       pic_width,
                       pic_width};
    SDL_Rect dest_rect {static_cast<int>(x), static_cast<int>(y), 
                        pic_width, pic_width};
    renderer.RenderSprite(texture, src_rect, dest_rect);
}

void Explosion::UpdateCounter() {
    int total_duration = pic_count * pic_interval;
    if (counter < total_duration) {
        ++counter;
    }
}

bool Explosion::Disappeared() {
    return (counter == pic_count * pic_interval);
}
