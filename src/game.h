#pragma once

#include <random>
#include <vector>

#include "SDL.h"
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
    SDL_Texture *player_texture;
    SDL_Texture *player_bullet_texture;
    SDL_Texture *enemy_texture;
    int enemySpwanTimer;
    
    void UpdatePlayerObjects(Controller::Actions const &action, Fighter &player, 
                             std::vector<Bullet> &bullets);
    void UpdateNonplayerObjects(std::vector<Fighter> &enemies,
                                std::vector<Bullet> &bullets,
                                std::mt19937 &eng);

    void PreloadTextures();

    bool CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2);
};