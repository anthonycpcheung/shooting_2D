#include "bullet.h"

Bullet::Bullet(SDL_Texture *sprite_texture, int speed)
    : Sprite{sprite_texture, speed}
{}

Bullet::Bullet(SDL_Texture *sprite_texture, int speed, 
               int initial_x, int initial_y)
    : Sprite{sprite_texture, speed, initial_x, initial_y}
{}
