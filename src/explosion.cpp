#include "explosion.h"

Explosion::Explosion(SDL_Texture *texture, std::size_t pic_count, int pic_interval)
    : GameObject{texture, 0}, 
    pic_count{pic_count}, pic_interval{pic_interval}, 
    counter{0}
{}

SDL_Rect Explosion::GetRect() const {
    return SDL_Rect{static_cast<int>(x), 
                    static_cast<int>(y), 
                    static_cast<int>(w / pic_count), 
                    h};
}

void Explosion::Render(Renderer &renderer) const {
    // according to current counter, pick the correct explosion pic and show
    int pic_width = w / pic_count;
    int pic_num = counter / pic_count;
    SDL_Rect src_rect {(pic_num * pic_width),
                       0,
                       pic_width,
                       h};
    SDL_Rect dest_rect {static_cast<int>(x), static_cast<int>(y), 
                        pic_width, h};
    renderer.RenderSprite(texture, src_rect, dest_rect);
}

void Explosion::UpdateCounter() {
    int total_duration = pic_count * pic_interval;
    if (counter < total_duration) {
        ++counter;
    }
}

bool Explosion::Disappeared() {
    // if the last pic of explosion shown, then this is the end of explosion
    return (counter == pic_interval * static_cast<int>(pic_count));
}
