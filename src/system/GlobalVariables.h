#ifndef MAGEQUEST_SRC_SYSTEM_GLOBALVARIABLES_H_
#define MAGEQUEST_SRC_SYSTEM_GLOBALVARIABLES_H_

#include "../graphics/elements/GifDrawer.h"

/* |-----------------------------------------------------|
 * |                        SYSTEM                       |
 * |-----------------------------------------------------|
 */
inline static char TEXT_BUFFER[50] = {0};
inline static float SCREEN_WIDTH = 1280;
inline static float SCREEN_HEIGHT = 960;
inline static int SCREEN_TILE_WIDTH = 42;
inline static int SCREEN_TILE_HEIGHT = 24;
inline static GameState GAME_STATE = GameState::Loading;
inline static std::shared_mutex rwLock;
inline const static std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();
inline static GifDrawer* RAYLIB_LOGO = nullptr;
inline static std::random_device Seed;
inline static std::mt19937 RNG_ENGINE(Seed());
inline static std::default_random_engine RNG_RANDOM(Seed());
inline static std::uniform_int_distribution<int> RANGE_EXISTING_ITEMS(0, 100);
inline static std::uniform_int_distribution<int> RANGE_ItemQuality(70, 100);
inline static std::uniform_int_distribution<int> RANGE_100(0, 100);
inline static std::uniform_real_distribution<float> RANGE_01(0, 1);
inline static std::uniform_real_distribution<float> RANGE_100_FLOAT(0, 100);
inline static std::uniform_real_distribution<float> distribution(0.0, 1.0);
inline static bool WINDOW_FOCUSED = false;
inline static int PERF_FRAMES = 0;
inline static long long PERF_TIME = 0;
inline static long long GAME_TICK_TIME = 0;
inline static long long FRAME_TIME = 0;
inline static Vector2 MOUSE_POS = GetMousePosition();
inline static bool FRIENDLY_FIRE = false;
#include "../gameplay/GameStatistics.h"
inline static GameStatistics GAME_STATISTICS{};
inline static constexpr float UPDATE_DISTANCE = 32;
inline static RenderTexture FIRST_LAYER_BUFFER;

/* |-----------------------------------------------------|
 * |                      STEAM                          |
 * |-----------------------------------------------------|
 */

inline static CSteamID LOBBY_ID;
inline static SteamNetworkingIdentity HOST_ID = SteamNetworkingIdentity();
inline static CSteamID PLAYER_ID;

/* |-----------------------------------------------------|
 * |                    PLAYER                      |
 * |-----------------------------------------------------|
 */
inline static constexpr int MAX_STATUS_EFFECTS_PRJ = 3;
#include "../gameplay/Stats.h"
#include "../gameplay/StatusEffect.h"
#include "../gameplay/handlers/StatusEffectHandler.h"
inline static float CAMERA_X = SCREEN_WIDTH / 2;
inline static float CAMERA_Y = SCREEN_HEIGHT / 2;
inline static float PLAYER_X = 0;
inline static float PLAYER_Y = 0;
inline static float DRAW_X = 0;
inline static float DRAW_Y = 0;
inline static float MIRROR_POINT = 0;
inline static PointT<int16_t>* PLAYER_TILE = nullptr;
inline static StatusEffectHandler PLAYER_EFFECTS{PLAYER_STATS};
inline static std::string PLAYER_NAME;
inline static Item* DRAGGED_ITEM = nullptr;
inline static Item* TOOL_TIP_ITEM = nullptr;
/* |-----------------------------------------------------|
 * |                          WORLD                      |
 * |-----------------------------------------------------|
 */
struct Projectile;
struct WorldObject;
struct NPC;
struct Monster;
struct Player;
inline static MultiplayerType MP_TYPE = MultiplayerType::OFFLINE;
inline static uint_fast32_t MONSTER_ID = 1;
inline static int16_t** CURRENT_BACK_GROUND = nullptr;
inline static int16_t** CURRENT_MIDDLE_GROUND = nullptr;
inline static int16_t** CURRENT_FORE_GROUND = nullptr;
inline static bool** CURRENT_MAP_COVER = nullptr;
inline static std::vector<SpawnTrigger>* CURRENT_SPAWN_TRIGGERS;
inline static Zone CURRENT_ZONE = Zone::Tutorial;
inline static int CURRENT_MAP_SIZE = 100;
inline static int8_t COLLISIONS[6000] = {0};
inline static uint16_t TILES[6000] = {0};
inline static std::vector<Map> MAPS;
inline static std::vector<Item> ITEMS;
inline static std::vector<Projectile*> PROJECTILES;
inline static std::vector<WorldObject*> WORLD_OBJECTS;
inline static std::vector<NPC*> NPCS;
inline static std::vector<Monster*> MONSTERS;

inline static bool CheckTileCollision(int x, int y) noexcept {
  return COLLISIONS[CURRENT_BACK_GROUND[x][y]] == C_SOLID ||
         COLLISIONS[CURRENT_MIDDLE_GROUND[x][y]] == C_SOLID;
}
inline static bool BoundCheckMap(int x, int y) noexcept {
  return x >= 0 && y >= 0 && x < CURRENT_MAP_SIZE && y < CURRENT_MAP_SIZE;
}
inline static bool IsTileCovered(int x, int y) noexcept {
  return CURRENT_MAP_COVER[x][y];
}

#endif  //MAGEQUEST_SRC_SYSTEM_GLOBALVARIABLES_H_
