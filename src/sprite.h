#pragma once

#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "renderer.h"

class Sprite {
    public:
    Sprite(SDL_Texture *sprite_texture, int speed);
    Sprite(SDL_Texture *sprite_texture);

    virtual void Render(Renderer &renderer) const;

    virtual SDL_Rect GetRect() const;
    SDL_Texture *GetTexture() const;

    void SetPosition(double new_x, double new_y);
    void SetSpeed(int new_speed);
    int GetSpeed() const;

    void Move(double dx, double dy);
    void Move(bool up, bool down, bool left, bool right);
    void BoundAdjust(SDL_Rect const &bound_rect);

    protected:
    SDL_Texture *texture;
    double x, y;
    int w, h;
    int speed;
};
