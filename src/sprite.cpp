#include "sprite.h"

Sprite::Sprite(SDL_Texture *texture, int speed)
    : texture{texture}, speed{speed}, x{0}, y{0}, hit{false}
{
    // set width and height of sprite from the image
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Sprite::Sprite(SDL_Texture *texture, int speed, int initial_x, int initial_y)
    : texture{texture}, speed{speed}, x{initial_x}, y{initial_y}, hit{false}
{
    // set width and height of sprite from the image
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

SDL_Rect Sprite::GetRect() const {
    return SDL_Rect{x, y, w, h};
}

SDL_Texture *Sprite::GetTexture() const {
    return texture;
}

void Sprite::SetPosition(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}

void Sprite::SetHit() {
    hit = true;
}

bool Sprite::GetHit() const {
    return hit;
}

int Sprite::GetSpeed() const {
    return speed;
}

void Sprite::Move(MoveDir dir) {
    switch(dir) {
        case MoveDir::UP:
            y -= speed;
            break;
        case MoveDir::DOWN:
            y += speed;
            break;
        case MoveDir::LEFT:
            x -= speed;
            break;
        case MoveDir::RIGHT:
            x += speed;
            break;
        default:
            // nothing change
            break;
    }
}

void Sprite::MoveWithBoundFix(MoveDir dir, int bound_width, int bound_height) {
    Move(dir);

    if (x < 0) {
        x = 0;
    }
    else if ((x + w) > bound_width) {
        x = bound_width - w;
    }
    
    if (y < 0) {
        y = 0;
    }
    else if ((y + h) > bound_height) {
        y = bound_height - h;
    }
}
