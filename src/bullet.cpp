#include "bullet.h"

Bullet::Bullet(SDL_Texture *sprite_texture, int speed)
    : Sprite{sprite_texture, speed}, hit{false}
{}

void Bullet::Hit() {
    hit = true;
}

bool Bullet::IsHit() const {
    return hit;
}