#include "controller.h"
#include "SDL.h"

void Controller::HandleInput(bool &runningFlag) const {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                runningFlag = false;
                return;
            default:
                break; // start next loop
        }
    }
}