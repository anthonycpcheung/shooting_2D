#include "sprite.h"

Sprite::Sprite(SDL_Texture *texture)
    : texture{texture}, x{0}, y{0}
{
    // set width and height of sprite from the image
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

Sprite::Sprite(SDL_Texture *texture, int initial_x, int initial_y)
    : texture{texture}, x{initial_x}, y{initial_y}
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

void Sprite::MoveTo(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}