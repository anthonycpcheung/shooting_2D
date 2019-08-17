#include "controller.h"

Controller::Actions Controller::ProcessEvent() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                actions.QUIT = true;
                return actions;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    HandleKeyDown(event.key.keysym.sym);
                }
                break;
            case SDL_KEYUP:
                if (event.key.repeat == 0) {
                    HandleKeyUp(event.key.keysym.sym);
                }
            default:
                break;
        }
    }

    return actions;
}

void Controller::HandleKeyDown(SDL_Keycode const &keycode) {
    if (keycode == SDLK_UP) {
        actions.UP = true;
    }

    if (keycode == SDLK_DOWN) {
        actions.DOWN = true;
    }

    if (keycode == SDLK_LEFT) {
        actions.LEFT = true;
    }

    if (keycode == SDLK_RIGHT) {
        actions.RIGHT = true;
    }

    if (keycode == SDLK_LCTRL) {
        actions.FIRE = true;
    }
}

void Controller::HandleKeyUp(SDL_Keycode const &keycode) {
    if (keycode == SDLK_UP) {
        actions.UP = false;
    }

    if (keycode == SDLK_DOWN) {
        actions.DOWN = false;
    }

    if (keycode == SDLK_LEFT) {
        actions.LEFT = false;
    }

    if (keycode == SDLK_RIGHT) {
        actions.RIGHT = false;
    }

    if (keycode == SDLK_LCTRL) {
        actions.FIRE = false;
    }
}