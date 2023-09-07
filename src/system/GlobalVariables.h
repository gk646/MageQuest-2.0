#ifndef MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
#define MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_

//SYSTEM
inline float SCREEN_WIDTH = 1280;
inline float SCREEN_HEIGHT = 960;
inline GameState GAME_STATE = GameState::Loading;
inline std::shared_mutex rwLock;
inline std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();
inline GifDrawer* RAYLIB_LOGO;
inline std::random_device e;
inline std::mt19937 RNG_ENGINE(e());
inline std::uniform_int_distribution<int> ITEM_DIST(1, 80);
inline std::uniform_int_distribution<int> QUALITY_DIST(70, 100);

//UI

//PLAYER
inline float CAMERA_X = SCREEN_WIDTH / 2;
inline float CAMERA_Y = SCREEN_HEIGHT / 2;
inline float PLAYER_X = 0;
inline float PLAYER_Y = 0;
inline int PLAYER_TILE_X = 0;
inline int PLAYER_TILE_Y = 0;



//WORLD
inline int16_t** CURRENT_BACK_GROUND;
inline int16_t** CURRENT_MIDDLE_GROUND;
inline int16_t** CURRENT_FORE_GROUND;
inline Zone CURRENT_ZONE = Zone::Hillcrest;
inline int CURRENT_MAP_SIZE = 100;
inline int8_t COLLISIONS[6000];
inline Texture TEXTURES[6000];
inline std::vector<Item> ITEMS;
inline std::vector<InventorySlot> CHAR_BAG;
#include "../ui/player/CharacterBag.h"
inline std::vector<InventorySlot> EQUIPPED_ITEMS;

//Entities
#include "../gameobjects/Entity.h"
#include "../gameobjects/WorldObject.h"

#include "../gameobjects/Projectile.h"
#include "../ui/game/HealthBar.h"

#include "../gameobjects/Monster.h"
#include "../gameobjects/NPC.h"

inline std::vector<Projectile*> PROJECTILES;
#include "../ui/player/HotBar.h"
inline HotBar PLAYER_HOTBAR{6, 1};
inline StatusEffectHandler PLAYER_EFFECTS{PLAYER_STATS};
#include "../gameobjects/Player.h"
inline Player PLAYER{{150, 150}, {25, 25}};
inline std::vector<WorldObject*> WORLD_OBJECTS;

inline std::vector<Player> OTHER_PLAYERS;
inline std::vector<NPC*> NPCS;
inline std::vector<Monster*> MONSTERS;
inline std::vector<Map> MAPS;

//BENCHMARK
inline int PERF_FRAMES = 0;
inline long long PERF_TIME = 0;

inline long long GAME_TICK_TIME = 0;
inline long long FRAME_TIME = 0;

#endif  //MAGE_QUEST_SRC_UTIL_GLOBALVARIABLES_H_
