#include "controller.h"
#include "SDL.h"

void Controller::HandleInput(bool &quitFlag) const {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitFlag = true;
                return;
            default:
                break; // start next loop
        }
    }
}