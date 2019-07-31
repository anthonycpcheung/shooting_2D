#include "fighter.h"

int constexpr RELOAD_LIMIT{8};
int constexpr UNLIMIT_BULLETS{-1};

Fighter::Fighter(SDL_Texture *sprite_texture, int speed)
    : Sprite{sprite_texture, speed}, 
    reload_counter{0}, bullet_count{UNLIMIT_BULLETS}
{}

Fighter::Fighter(SDL_Texture *sprite_texture, int speed, int bullets)
    : Sprite{sprite_texture, speed}, 
    reload_counter{0}, bullet_count{bullets}
{}


bool Fighter::Fire(bool triggering) {
    if (reload_counter > 0) {
        --reload_counter;
    }

    bool fired = (reload_counter == 0 && triggering && 
                  (bullet_count > 0 || bullet_count == UNLIMIT_BULLETS));
    if (fired) {
        reload_counter = RELOAD_LIMIT;
        if (bullet_count > 0) {
            --bullet_count;
        }
    }
    return fired;
}
