#pragma once

#include <unordered_map>
#include <vector>

#include "bullet.h"
#include "controller.h"
#include "fighter.h"
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

    Sprite CreateSprite(std::string tag, int speed);
    void UpdatePlayerObjects(Controller::Actions const &action, Fighter &player, 
                             std::vector<Bullet> &bullets);
    void UpdateNonplayerObjects();
};