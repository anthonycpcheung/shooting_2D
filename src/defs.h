#pragma once

#include <string>

// Screen size and frame rate
std::size_t constexpr screenWidth{1280};
std::size_t constexpr screenHeight{720};
std::size_t constexpr framePerSecond{60};
std::size_t constexpr msPerFrame{1000 / framePerSecond};

// Textures path
std::string const PLAYER_IMAGE{"../gfx/player.png"};
std::string const PLAYER_BULLET_IMAGE{"../gfx/playerBullet.png"};
std::string const ENEMY_IMAGE("../gfx/enemy.png");
std::string const ENEMY_BULLET_IMAGE("../gfx/enemyBullet.png");
std::string const BACKGROUND_IMAGE("../gfx/background.png");
std::string const EXPLOSIONS_IMAGE("../gfx/explosions.png");
std::string const FONTS_IMAGE("../gfx/font.png");
std::string const TITLE_IMAGE("../gfx/gametitle.png");

// Speed in pixels per frame
int constexpr PLAYER_SPEED{4};
int constexpr BULLET_SPEED{8};
int constexpr MIN_ENEMY_SPEED{2};
int constexpr MAX_ENEMY_SPEED{6};
int constexpr BG_SCROLLING_SPEED{1};

// Life in # of hits can tolerate
int constexpr PLAYER_LIFE{1};
int constexpr ENEMY_LIFE{1};

// Time in # of frames
int constexpr PLAYER_RELOAD{8};
int constexpr MIN_SPWAN_TIME{30};
int constexpr MAX_SPWAN_TIME{90};
int constexpr MIN_FIRE_TIME{60};
int constexpr MAX_FIRE_TIME{180};

// Explosion
int constexpr EXPLOSION_PIC_COUNT{10};
int constexpr EXPLOSION_PIC_INTERVAL{12};

// Font
int constexpr FONT_WIDTH{18};
