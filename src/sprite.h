#pragma once

#include <string>

#include "SDL.h"
#include "SDL_image.h"

class Sprite {
    public:
    enum class MoveDir { UP, DOWN, LEFT, RIGHT };

    Sprite(SDL_Texture *sprite_texture, int speed);
    Sprite(SDL_Texture *sprite_texture, int speed, int initial_x, int initial_y);
    //~Sprite();

    // For used by Renderer to render the sprite
    SDL_Rect GetRect() const;
    SDL_Texture *GetTexture() const;

    void Move(MoveDir dir);
    void SetPosition(int new_x, int new_y);
    void SetShow(bool toShow);
    bool GetShow() const;
    int GetSpeed() const;

    private:
    SDL_Texture *texture;
    int x, y, w, h;
    int speed;
    bool show;
};