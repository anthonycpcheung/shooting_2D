#pragma once

#include "SDL.h"

class Controller {
    public:
    struct Actions {
        bool QUIT{false};
        bool UP{false};
        bool DOWN{false};
        bool LEFT{false};
        bool RIGHT{false};
        bool FIRE{false};
    };

    Actions ProcessEvent();

    private:
    Actions actions;

    void HandleKeyDown(SDL_Keycode const &keycode);
    void HandleKeyUp(SDL_Keycode const &keycode);
};