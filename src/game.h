#pragma once

#include <iostream>
#include <vector>

#include "controller.h"
#include "renderer.h"
#include "sprite.h"

class Game {
    public:
    Game(Renderer &renderer, Controller &controller);
    ~Game() {}
    void Run(std::size_t target_frame_duration);

    private:
    Renderer &renderer;
    Controller &controller;
    std::vector<Sprite> sprites;

    Sprite CreateSprite(std::string sprite_image_filename, int speed);
    void UpdatePlayerObjects(Controller::Actions const &action);
    void UpdateNonplayerObjects();
};