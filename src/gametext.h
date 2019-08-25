#pragma once

#include <string>

#include "gameobject.h"

/*
 This class represents a text string to displayed on game screen. 
 */
class GameText : public GameObject {
    public:
    GameText(SDL_Texture *texture, int font_width);

    void Render(Renderer &renderer) const;
    SDL_Rect GetRect() const;

    // Set the text string to be displayed
    void SetText(std::string const &text);

    private:
    int font_width;
    std::string text;
};

