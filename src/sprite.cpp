#include "sprite.h"

Sprite::Sprite(SDL_Texture *texture, int speed)
    : texture{texture}, speed{speed}, x{0}, y{0}, show{false}
{
    // set width and height of sprite from the image
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Sprite::Sprite(SDL_Texture *texture, int speed, int initial_x, int initial_y)
    : texture{texture}, speed{speed}, x{initial_x}, y{initial_y}, show{false}
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

void Sprite::SetShow(bool toShow) {
    show = toShow;
}

bool Sprite::GetShow() const {
    return show;
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