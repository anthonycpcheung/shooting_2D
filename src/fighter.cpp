#include "fighter.h"

Fighter::Fighter(SDL_Texture *sprite_texture, int speed)
    : Sprite{sprite_texture, speed}, 
    reloadCounter{0}
{}

Fighter::Fighter(SDL_Texture *sprite_texture, int speed, 
                 int initial_x, int initial_y)
    : Sprite{sprite_texture, speed, initial_x, initial_y}, 
    reloadCounter{0}
{}

bool Fighter::CheckReload() {
    if (reloadCounter > 0) {
        --reloadCounter;
    }
    return (reloadCounter == 0);
}

void Fighter::Fired() {
    int constexpr RELOAD_LIMIT{8};
    reloadCounter = RELOAD_LIMIT;
}
