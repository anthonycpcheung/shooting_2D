#include <cmath>

#include "sprite.h"

Sprite::Sprite(SDL_Texture *texture, int speed)
    : texture{texture}, speed{speed}, x{0}, y{0}
{
    // set width and height of sprite from the image
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

void Sprite::Render(Renderer &renderer) const {
    renderer.RenderSprite(texture, GetRect());
}

SDL_Rect Sprite::GetRect() const {
    return SDL_Rect{static_cast<int>(x), static_cast<int>(y), w, h};
}

SDL_Texture *Sprite::GetTexture() const {
    return texture;
}

void Sprite::SetPosition(double new_x, double new_y) {
    x = new_x;
    y = new_y;
}
void Sprite::SetSpeed(int new_speed) {
    speed = new_speed;
}

int Sprite::GetSpeed() const {
    return speed;
}

void Sprite::Move(double dx, double dy) {
    x += dx;
    y += dy;
}

void Sprite::Move(bool up, bool down, bool left, bool right) {
    double dx = 0;
    double dy = 0;

    dy = (up) ? (dy - 1) : dy;
    dy = (down) ? (dy + 1) : dy;
    dx = (left) ? (dx - 1) : dx;
    dx = (right) ? (dx + 1) : dx;

    double normalize_factor = 1;
    if (dx != 0 && dy != 0) {
        normalize_factor = sqrt((dx * dx) + (dy * dy));
    }

    x += dx / normalize_factor * speed;
    y += dy / normalize_factor * speed;
}

void Sprite::BoundAdjust(SDL_Rect const &bound_rect) {
    if (x < 0) {
        x = 0;
    }
    else if (x + w > bound_rect.w) {
        x = bound_rect.w - w;
    }

    if (y < 0) {
        y = 0;
    }
    else if (y + h > bound_rect.h) {
        y = bound_rect.h - h;
    }
}
