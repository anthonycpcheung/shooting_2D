#include "gametext.h"

GameText::GameText(SDL_Texture *texture, int font_width)
    : GameObject(texture, 0), font_width{font_width}
{}

void GameText::Render(Renderer &renderer) const {
    auto pos_x = static_cast<int>(x);
    auto pos_y = static_cast<int>(y);

    // loop through each character in text and render it 
    for (std::size_t i = 0; i < text.length(); ++i) {
        auto c = text[i];
        if (c >= ' ' && c <= 'Z') {
            auto font_x = (c - ' ') * font_width;
            SDL_Rect src_rect{font_x, 0, font_width, h};
            SDL_Rect dest_rect{pos_x, pos_y, font_width, h};

            renderer.RenderSprite(texture, src_rect, dest_rect);

            pos_x += font_width;
        }
    }
}

SDL_Rect GameText::GetRect() const {
    return SDL_Rect {static_cast<int>(x), static_cast<int>(y), 
                     font_width * static_cast<int>(text.length()), h};
}

void GameText::SetText(std::string const &text) {
    GameText::text = text;
}