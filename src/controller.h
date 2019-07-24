#pragma once

#include "sprite.h"

class Controller {
    public:
    void HandleInput(bool &quitFlag, Sprite &player) const;

    private:
    void MovePlayer(Sprite &player, SDL_Keycode const key) const;
};