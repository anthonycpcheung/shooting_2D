#pragma once

#include <forward_list>
#include <random>

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
    SDL_Texture *enemy_bullet_texture;
    int enemySpwanTimer;
    
    void UpdatePlayerObjects(Controller::Actions const &action, Fighter &player, 
                             std::forward_list<Bullet> &bullets);
    void UpdateNonplayerObjects(std::forward_list<Fighter> &enemies,
                                std::forward_list<Bullet> &bullets,
                                std::mt19937 &eng);
    void BulletsHitEnemies(std::forward_list<Bullet> &bullets, 
                          std::forward_list<Fighter> &enemies);
    void BulletsHitBullets(std::forward_list<Bullet> &player_bullets, 
                          std::forward_list<Bullet> &enemy_bullets);
    void BulletsHitPlayer(std::forward_list<Bullet> &player_bullets, 
                          Fighter &player);
    void EnemiesHitPlayer(std::forward_list<Fighter> &enemies, 
                          Fighter &player);
    void ClearInvalidObjects(std::forward_list<Fighter> &enemies,
                             std::forward_list<Bullet> &player_bullets,
                             std::forward_list<Bullet> &enemy_bullets);

    void PreloadTextures();

    bool CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2);
};