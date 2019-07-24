#include "controller.h"
#include "SDL.h"

void Controller::HandleInput(bool &runningFlag, Sprite &player) const {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                runningFlag = false;
                return;
            case SDL_KEYDOWN:
                MovePlayer(player, event.key.keysym.sym);
            default:
                break; // start next loop
        }
    }
}

void Controller::MovePlayer(Sprite &player, SDL_Keycode const keycode) const {
    switch(keycode) {
        case SDLK_UP:
            player.Move(Sprite::MoveDir::UP);
            break;
        case SDLK_DOWN:
            player.Move(Sprite::MoveDir::DOWN);
            break;
        case SDLK_LEFT:
            player.Move(Sprite::MoveDir::LEFT);
            break;
        case SDLK_RIGHT:
            player.Move(Sprite::MoveDir::RIGHT);
            break;
        default:
            // do nothing
            break;
    }
}