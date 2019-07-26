#include "fighter.h"

Fighter::Fighter(SDL_Texture *sprite_texture, int speed, int reload)
    : Sprite{sprite_texture, speed}, 
    reload{reload}, reloadCounter{0}
{}

Fighter::Fighter(SDL_Texture *sprite_texture, int speed, 
                 int initial_x, int initial_y, int reload)
    : Sprite{sprite_texture, speed, initial_x, initial_y}, 
    reload{reload}, reloadCounter{0}
{}

bool Fighter::CheckReload() {
    if (reloadCounter > 0) {
        --reloadCounter;
    }
    return (reloadCounter == 0);
}

void Fighter::Fired() {
    reloadCounter = reload;
}
