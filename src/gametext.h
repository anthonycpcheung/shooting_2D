#pragma once

#include <string>

#include "sprite.h"

class GameText : public Sprite {
    public:
    GameText(SDL_Texture *texture, int font_width);

    void Render(Renderer &renderer) const;
    SDL_Rect GetRect() const;
    void SetText(std::string const &text);

    private:
    int font_width;
    std::string text;
};

