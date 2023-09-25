#ifndef MAGEQUEST_SRC_SYSTEM_GLOBALVARIABLES_H_
#define MAGEQUEST_SRC_SYSTEM_GLOBALVARIABLES_H_

#include "../graphics/GifDrawer.h"

/* |-----------------------------------------------------|
 * |                        SYSTEM                       |
 * |-----------------------------------------------------|
 */

inline static float SCREEN_WIDTH = 1280;
inline static float SCREEN_HEIGHT = 960;
inline static GameState GAME_STATE = GameState::Loading;
inline static std::shared_mutex rwLock;
inline static std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();
inline static GifDrawer* RAYLIB_LOGO = nullptr;
inline static LogoDrawer* NBNET_LOGO = nullptr;
inline static std::random_device e;
inline static std::mt19937 RNG_ENGINE(e());
inline static std::default_random_engine RNG_RANDOM;
inline static std::uniform_int_distribution<int> RANGE_EXISTING_ITEMS(1, 80);
inline static std::uniform_int_distribution<int> RANGE_ItemQuality(70, 100);
inline static std::uniform_int_distribution<int> RANGE_100(0, 100);
inline static std::uniform_real_distribution<float> RANGE_01(0, 1);
inline static std::uniform_real_distribution<float> RANGE_01SMALL(-0.25, 0.25);
inline static std::uniform_real_distribution<float> distribution(0.0, 1.0);
inline bool WINDOW_FOCUSED = false;
inline static int PERF_FRAMES = 0;
inline static long long PERF_TIME = 0;
inline static long long GAME_TICK_TIME = 0;
inline static long long FRAME_TIME = 0;
inline static Vector2 MOUSE_POS = GetMousePosition();
inline static bool FRIENDLY_FIRE = false;

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
inline static float CAMERA_X = SCREEN_WIDTH / 2;
inline static float CAMERA_Y = SCREEN_HEIGHT / 2;
inline static float PLAYER_X = 0;
inline static float PLAYER_Y = 0;
inline static float DRAW_X = 0;
inline static float DRAW_Y = 0;
inline static float MIRROR_POINT = 0;
inline static PointI* PLAYER_TILE = nullptr;
inline static StatusEffectHandler PLAYER_EFFECTS{PLAYER_STATS};
inline static std::string PLAYER_NAME;
class Item;
inline Item* DRAGGED_ITEM = nullptr;
inline Item* TOOL_TIP_ITEM = nullptr;
/* |-----------------------------------------------------|
 * |                          WORLD                      |
 * |-----------------------------------------------------|
 */
class Projectile;
class WorldObject;
class NPC;
class Monster;
class Player;
inline uint_fast32_t MONSTER_ID = 1;
inline uint_fast32_t PROJECTILE_ID = 1;
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
inline static std::vector<Projectile*> PROJECTILES;
inline static std::vector<WorldObject*> WORLD_OBJECTS;
inline static std::vector<NPC*> NPCS;
inline static std::vector<Monster*> MONSTERS;

inline static bool CheckTileCollision(int x, int y ) noexcept {
  return COLLISIONS[CURRENT_BACK_GROUND[x][y]] == C_SOLID &&
         COLLISIONS[CURRENT_MIDDLE_GROUND[x][y]] == C_SOLID;
}
#endif  //MAGEQUEST_SRC_SYSTEM_GLOBALVARIABLES_H_
