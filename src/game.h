#pragma once

#include <forward_list>
#include <memory>
#include <random>

#include "SDL.h"
#include "bullet.h"
#include "controller.h"
#include "fighter.h"
#include "renderer.h"
#include "sprite.h"
#include "background.h"
#include "explosion.h"
#include "gametext.h"

class Game {
    public:
    Game(Renderer &renderer, Controller &controller);
    ~Game() {}
    void Run(std::size_t target_frame_duration);

    private:
    Renderer &renderer;
    Controller &controller;
    SDL_Texture *background_texture;
    SDL_Texture *player_texture;
    SDL_Texture *player_bullet_texture;
    SDL_Texture *enemy_texture;
    SDL_Texture *enemy_bullet_texture;
    SDL_Texture *explosions_texture;
    SDL_Texture *fonts_texture;
    SDL_Texture *title_texture;

    int enemySpwanTimer;
    int score;
    bool game_over;

    // Game objects
    std::unique_ptr<Background> background;
    std::unique_ptr<Sprite> game_title;
    std::unique_ptr<GameText> game_message;
    std::unique_ptr<Fighter> player;
    std::forward_list<std::unique_ptr<Bullet> > player_bullets;
    std::forward_list<std::unique_ptr<Fighter> > enemies;
    std::forward_list<std::unique_ptr<Bullet> > enemy_bullets;
    std::forward_list<std::unique_ptr<Explosion> > explosions;
    
    void PreloadTextures();
    void SetShowTtile();
    void ResetGame();
    void RenderGameTitleScreen();
    void RenderGamePlayScreen();
    void UpdatePlayerObjects(Controller::Actions const &action);
    void UpdateEnemyObjects(std::mt19937 &eng);
    void UpdateExplosions();
    void BulletsHitEnemies();
    void BulletsHitBullets();
    void BulletsHitPlayer();
    void EnemiesHitPlayer();
    void ClearInvalidObjects();
    void CreateExplosion(Fighter const &fighter_got_hit);
    void UpdateScoreText();

    bool CheckCollision(SDL_Rect const &rect1, SDL_Rect const &rect2);
};