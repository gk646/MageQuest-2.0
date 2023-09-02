#ifndef MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
#define MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_

#include <cstdint>
#include <string>

#include "Definitions.h"

#include "../graphics/maps/Map.h"
#include "../util/Colors.h"
#include "../util/Enums.h"

//PLAYER
inline int SCREEN_WIDTH = 1280;
inline int SCREEN_HEIGHT = 960;
inline float UI_SCALE = 1;
inline int CAMERA_X = SCREEN_WIDTH / 2;
inline int CAMERA_Y = SCREEN_HEIGHT / 2;
inline int PLAYER_X = 0;
inline int PLAYER_Y = 0;
inline int PLAYER_TILE_X = 0;
inline int PLAYER_TILE_Y = 0;

inline GameState GAME_STATE = GameState::Loading;

inline std::string ASSET_PATH = "res/";

//WORLD
inline int16_t** CURRENT_BACK_GROUND;
inline int16_t** CURRENT_MIDDLE_GROUND;
inline int16_t** CURRENT_FORE_GROUND;
inline int CURRENT_MAP_SIZE = 100;

inline Texture2D TEXTURES[6000];
inline int COLLISIONS[6000];

inline Difficulty GAME_DIFFICULTY;

//Entities
#include "../gameobjects/entities/types/Monster.h"
#include "../gameobjects/entities/types/NPC.h"
#include "../gameobjects/entities/types/Player.h"

std::vector<Player> OTHER_PLAYERS;
std::vector<Projectile> PROJECTILES;
std::vector<NPC> NPCS;
std::vector<Monster> MONSTERS;

inline std::vector<Map> MAPS{};

//BENCHMARK
inline int PERF_FRAMES = 0;
inline long long PERF_TIME = 0;


inline long long GAME_TICK_TIME = 0;
inline long long FRAME_TIME = 0;

#endif  //MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
