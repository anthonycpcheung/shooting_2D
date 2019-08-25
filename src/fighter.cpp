#include "fighter.h"

Fighter::Fighter(SDL_Texture *texture, int speed, int life)
    : GameObject{texture, speed}, 
    reload_count{0}, life_count{life}
{}

Fighter::Fighter(SDL_Texture *texture, int speed, int life, int reload)
    : GameObject{texture, speed}, 
    reload_count{reload}, life_count{life}
{}


bool Fighter::Fire(bool triggering) {
    if (reload_count > 0) {
        --reload_count;
    }

    return (reload_count <= 0 && triggering);
}

void Fighter::Reload(int count) {
    if (reload_count <= 0) {
        reload_count = count;
    }
}

void Fighter::GotHit() {
    if (life_count > 0) {
        --life_count;
    }
}

bool Fighter::IsDead() {
    return (life_count <= 0);
}