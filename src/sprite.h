#pragma once

#include <string>

#include "SDL.h"
#include "SDL_image.h"

enum class MoveDir { UP, DOWN, LEFT, RIGHT };

class Sprite {
    public:
    Sprite(SDL_Texture *sprite_texture, int speed);
    Sprite(SDL_Texture *sprite_texture, int speed, 
           int initial_x, int initial_y);

    // For used by Renderer to render the sprite
    SDL_Rect GetRect() const;
    SDL_Texture *GetTexture() const;

    void Move(MoveDir dir);
    void MoveWithBoundFix(MoveDir dir, int bound_width, int bound_height);
    void SetPosition(int new_x, int new_y);
    int GetSpeed() const;
    void SetHit();
    bool GetHit() const;

    private:
    SDL_Texture *texture;
    int x, y, w, h;
    int speed;
    bool hit;
};

class Fighter : public Sprite {
    public:
    Fighter(SDL_Texture *sprite_texture, int speed, int reload);
    Fighter(SDL_Texture *sprite_texture, int speed, 
            int initial_x, int initial_y, int reload);

    bool CheckReload();
    void Fired();

    private:
    int const reload;
    int reloadCounter;
};

class Bullet : public Sprite {
    public:
    Bullet(SDL_Texture *sprite_texture, int speed);
    Bullet(SDL_Texture *sprite_texture, int speed, 
            int initial_x, int initial_y);
};