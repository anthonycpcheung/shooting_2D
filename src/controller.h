#pragma once

#include "SDL.h"

class Controller {
    public:
    struct Actions {
        bool UP{false};
        bool DOWN{false};
        bool LEFT{false};
        bool RIGHT{false};
        bool FIRE{false};
    };

    void ProcessEvent(bool &quitFlag, Actions &actions) const;

    private:
    void HandleKeyDown(SDL_Keycode const &keycode, Actions &actions) const;
    void HandleKeyUp(SDL_Keycode const &keycode, Actions &actions) const;
};