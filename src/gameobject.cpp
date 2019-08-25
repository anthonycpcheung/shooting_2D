#include <cmath>

#include "gameobject.h"

GameObject::GameObject(SDL_Texture *texture, int speed)
    : texture{texture}, speed{speed}, x{0}, y{0}
{
    // set width and height of GameObject from the image
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
}

GameObject::GameObject(SDL_Texture *texture)
    : GameObject{texture, 0}
{}

void GameObject::Render(Renderer &renderer) const {
    renderer.RenderSprite(texture, GetRect());
}

SDL_Rect GameObject::GetRect() const {
    return SDL_Rect{static_cast<int>(x), static_cast<int>(y), w, h};
}

SDL_Texture *GameObject::GetTexture() const {
    return texture;
}

void GameObject::SetPosition(double new_x, double new_y) {
    x = new_x;
    y = new_y;
}
void GameObject::SetSpeed(int new_speed) {
    speed = new_speed;
}

int GameObject::GetSpeed() const {
    return speed;
}

void GameObject::Move(double dx, double dy) {
    x += dx;
    y += dy;
}

void GameObject::Move(bool up, bool down, bool left, bool right) {
    double dx = 0;
    double dy = 0;

    dy = (up) ? (dy - 1) : dy;
    dy = (down) ? (dy + 1) : dy;
    dx = (left) ? (dx - 1) : dx;
    dx = (right) ? (dx + 1) : dx;

    // if the object is moving diagonally, the displacement is calculated by 
    // pythagoras theorem.
    double normalize_factor = 1;
    if (dx != 0 && dy != 0) {
        normalize_factor = sqrt((dx * dx) + (dy * dy));
    }

    x += dx / normalize_factor * speed;
    y += dy / normalize_factor * speed;
}

void GameObject::BoundAdjust(SDL_Rect const &bound_rect) {
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
