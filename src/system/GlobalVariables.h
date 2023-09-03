#ifndef MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
#define MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_

//SYSTEM
inline float SCREEN_WIDTH = 1280;
inline float SCREEN_HEIGHT = 960;
inline GameState GAME_STATE = GameState::Loading;
inline std::string ASSET_PATH = "res/";
inline std::shared_mutex rwLock;

//PLAYER
inline float UI_SCALE = 1;
inline float CAMERA_X = SCREEN_WIDTH / 2;
inline float CAMERA_Y = SCREEN_HEIGHT / 2;
inline float PLAYER_X = 0;
inline float PLAYER_Y = 0;
inline int PLAYER_TILE_X = 0;
inline int PLAYER_TILE_Y = 0;

inline int COLLISIONS[6000];

//WORLD
inline int16_t** CURRENT_BACK_GROUND;
inline int16_t** CURRENT_MIDDLE_GROUND;
inline int16_t** CURRENT_FORE_GROUND;
inline int CURRENT_MAP_SIZE = 100;

inline Texture2D TEXTURES[6000];

inline Difficulty GAME_DIFFICULTY = Difficulty::NORMAL;

//Entities
#include "../gameobjects/Entity.h"

#include "../gameobjects/Projectile.h"
#include "../ui/game/HealthBar.h"

#include "../gameobjects/Monster.h"
#include "../gameobjects/NPC.h"

inline EntityStats PLAYER_STATS;
inline std::list<Projectile> PROJECTILES;
#include "../ui/player/HotBar.h"
inline HotBar PLAYER_HOTBAR{5, 1};

#include "../gameobjects/Player.h"
inline Player PLAYER{{150, 150}, {25, 25}};

inline std::vector<Player> OTHER_PLAYERS;
inline std::vector<NPC> NPCS;
inline std::vector<Monster> MONSTERS;

inline std::vector<Map> MAPS{};

//BENCHMARK
inline int PERF_FRAMES = 0;
inline long long PERF_TIME = 0;

inline long long GAME_TICK_TIME = 0;
inline long long FRAME_TIME = 0;

#endif  //MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
