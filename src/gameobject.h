#pragma once

#include <string>

#include "SDL.h"
#include "renderer.h"

/*
 This class represents a game object which can be shown on game screen. 
 */
class GameObject {
    public:
    GameObject(SDL_Texture *texture, int speed);
    GameObject(SDL_Texture *texture);

    // Render the object itself by provided Renderer
    virtual void Render(Renderer &renderer) const;

    virtual SDL_Rect GetRect() const;
    SDL_Texture *GetTexture() const;

    // Directly set the position of the object
    void SetPosition(double new_x, double new_y);

    // Set or Get speed of the objct
    void SetSpeed(int new_speed);
    int GetSpeed() const;

    // Move the object by provided displacemnt
    void Move(double dx, double dy);

    // Move th object in provided directions.
    // Displacement is calculated according to the spped of the object.
    void Move(bool up, bool down, bool left, bool right);

    // Check whether the object is out of the bound area.
    // If yes, adjust its position back to within the bound area.
    void BoundAdjust(SDL_Rect const &bound_rect);

    protected:
    SDL_Texture *texture;
    int speed;
    double x, y;
    int w, h;
};
