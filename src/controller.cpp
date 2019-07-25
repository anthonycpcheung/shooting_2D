#include "controller.h"

void Controller::ProcessEvent(bool &runningFlag, Actions &actions) const {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                runningFlag = false;
                return;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    HandleKeyDown(event.key.keysym.sym, actions);
                }
                break;
            case SDL_KEYUP:
                if (event.key.repeat == 0) {
                    HandleKeyUp(event.key.keysym.sym, actions);
                }
            default:
                break; // start next loop
        }
    }
}

void Controller::HandleKeyDown(SDL_Keycode const &keycode, Actions &actions) const {
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

    if (keycode == SDLK_SPACE) {
        actions.FIRE = true;
    }
}

void Controller::HandleKeyUp(SDL_Keycode const &keycode, Actions &actions) const {
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

    if (keycode == SDLK_SPACE) {
        actions.FIRE = false;
    }
}