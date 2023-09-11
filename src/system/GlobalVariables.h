#ifndef MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
#define MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_

//SYSTEM
inline static float SCREEN_WIDTH = 1280;
inline static float SCREEN_HEIGHT = 960;
inline static GameState GAME_STATE = GameState::Loading;
inline static std::shared_mutex rwLock;
inline static std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();
inline static GifDrawer* RAYLIB_LOGO;
inline static std::random_device e;
inline static std::mt19937 RNG_ENGINE(e());
inline static std::default_random_engine RNG_RANDOM;
inline static std::uniform_int_distribution<int> RANGE_EXISTING_ITEMS(1, 80);
inline static std::uniform_int_distribution<int> RANGE_ItemQuality(70, 100);
inline static std::uniform_int_distribution<int> RANGE_100(0, 100);
inline static std::uniform_real_distribution<float> RANGE_01(0, 1);
inline static std::uniform_real_distribution<float> RANGE_01SMALL(-0.25, 0.25);
// Distribution in the range [0, 1)
inline static std::uniform_real_distribution<float> distribution(0.0, 1.0);
//UI

//PLAYER
inline static float CAMERA_X = SCREEN_WIDTH / 2;
inline static float CAMERA_Y = SCREEN_HEIGHT / 2;
inline static float PLAYER_X = 0;
inline static float PLAYER_Y = 0;
inline static float DRAW_X = 0;
inline static float DRAW_Y = 0;
inline static float MIRROR_POINT = 0;
inline static PointI* PLAYER_TILE = nullptr;

//WORLD
inline static int16_t** CURRENT_BACK_GROUND;
inline static int16_t** CURRENT_MIDDLE_GROUND;
inline static int16_t** CURRENT_FORE_GROUND;
inline static int8_t** CURRENT_MAP_COVER;
inline static Zone CURRENT_ZONE = Zone::Hillcrest;
inline static int CURRENT_MAP_SIZE = 100;
inline static int8_t COLLISIONS[6000];
inline static Texture TEXTURES[6000];
inline static std::vector<Map> MAPS;

inline static std::vector<Item> ITEMS;
#include "../ui/player/CharacterBag.h"

//Entities
#include "../gameobjects/Entity.h"
#include "../gameobjects/WorldObject.h"

#include "../gameobjects/Projectile.h"
#include "../ui/game/HealthBar.h"

#include "../multiplayer/NetPlayer.h"

#include "../gameobjects/NPC.h"
#include "../multiplayer/MessageTypes.h"

inline static std::vector<Projectile*> PROJECTILES;
#include "../ui/player/HotBar.h"
inline static HotBar PLAYER_HOTBAR{6, 1};
inline static StatusEffectHandler PLAYER_EFFECTS{PLAYER_STATS};
#include "../multiplayer/Multiplayer.h"
#include "../gameobjects/Player.h"
inline static Player PLAYER{{150, 150}, {25, 25}};
inline static std::vector<WorldObject*> WORLD_OBJECTS;
#include "../world/WorldManager.h"
#include "../multiplayer/menus/ServerBrowser.h"


#include "../gameobjects/Monster.h"
inline static std::vector<NPC*> NPCS;
inline static std::vector<Monster*> MONSTERS;

//BENCHMARK
inline static int PERF_FRAMES = 0;
inline static long long PERF_TIME = 0;

inline static long long GAME_TICK_TIME = 0;
inline static long long FRAME_TIME = 0;

#endif  //MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
