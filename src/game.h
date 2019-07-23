#pragma once

#include <iostream>

#include "controller.h"
#include "renderer.h"

class Game {
    public:
    Game(Renderer &renderer, Controller const &controller);
    ~Game() {}
    void Run();

    private:
    Renderer &renderer;
    Controller const &controller;
};