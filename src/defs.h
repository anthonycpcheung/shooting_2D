#pragma once

#include <string>

// Screen size and frame rate
inline std::size_t constexpr screenWidth{1280};
inline std::size_t constexpr screenHeight{720};
inline std::size_t constexpr framePerSecond{60};
inline std::size_t constexpr msPerFrame{1000 / framePerSecond};

// Textures path
inline std::string const PLAYER_IMAGE{"../gfx/player.png"};
inline std::string const PLAYER_BULLET_IMAGE{"../gfx/playerBullet.png"};
inline std::string const ENEMY_IMAGE("../gfx/enemy.png");
inline std::string const ENEMY_BULLET_IMAGE("../gfx/enemyBullet.png");
inline std::string const BACKGROUND_IMAGE("../gfx/background.png");
inline std::string const EXPLOSIONS_IMAGE("../gfx/explosions.png");
inline std::string const FONTS_IMAGE("../gfx/font.png");
inline std::string const TITLE_IMAGE("../gfx/gametitle.png");

// Speed in pixels per frame
inline int constexpr PLAYER_SPEED{4};
inline int constexpr BULLET_SPEED{8};
inline int constexpr MIN_ENEMY_SPEED{2};
inline int constexpr MAX_ENEMY_SPEED{6};
inline int constexpr BG_SCROLLING_SPEED{1};

// Life in # of hits can tolerate
inline int constexpr PLAYER_LIFE{1};
inline int constexpr ENEMY_LIFE{1};

// Time in # of frames
inline int constexpr PLAYER_RELOAD{8};
inline int constexpr MIN_SPWAN_TIME{30};
inline int constexpr MAX_SPWAN_TIME{90};
inline int constexpr MIN_FIRE_TIME{60};
inline int constexpr MAX_FIRE_TIME{180};

// Explosion
inline int constexpr EXPLOSION_PIC_COUNT{10};
inline int constexpr EXPLOSION_PIC_INTERVAL{12};

// Font
inline int constexpr FONT_WIDTH{18};
