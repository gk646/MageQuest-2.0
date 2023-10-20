#ifndef MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_
#define MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_

/* |-----------------------------------------------------|
 * |                      SYSTEM                         |
 * |-----------------------------------------------------|
 */
#define SQRT_2 1.2142F
#define SIMD_PRAGMA _Pragma("omp simd")
#define TILE_SIZE 48
#define SCALE(val) ((val)*UI_SCALE)
#define SCALE_RECT(rect) \
  { SCALE((rect).x), SCALE((rect).y), SCALE((rect).width), SCALE((rect).height) }

#define SCALE_VEC(vec) \
  { SCALE((vec).x), SCALE((vec).y) }

inline std::string ASSET_PATH = "../res/";
/* |-----------------------------------------------------|
 * |                    MULTIPLAYER                      |
 * |-----------------------------------------------------|
 */
#define MG2_NET_IDENTIFIER "MageQuest"
#define MG2_PORT 42042
#define MG2_TICK_RATE 60
#define MG2_TICK_TIME 0.0166
#define MG2_MAX_CLIENTS 3
#define MG2_MAX_PLAYERS 4
//codes
#define MG2_SERVER_BUSY 401
#define MG2_HOST_CLOSED_GAMED 402
#define MP_MAX_MESSAGES_POS 4
#define MP_MAX_MESSAGES_MONSTER 60
#define MP_MAX_MESSAGES 20

/* |-----------------------------------------------------|
 * |                         GAME                        |
 * |-----------------------------------------------------|
 */
#define MAX_DYNAMIC_LIGHTS 50

/* |-----------------------------------------------------|
 * |                TILE COLLISIONS                      |
 * |-----------------------------------------------------|
 */
#define C_SOLID 1
#define C_AIR_ONLY 2
#define C_WATER 3


//UI
#define SLOT_SIZE 45
/* |-----------------------------------------------------|
 * |                       DEBUG                         |
 * |-----------------------------------------------------|
 */
#define MG2_DEBUG

#ifdef MG2_DEBUG
#define DRAW_HITBOXES     //Draws an outline around hitboxes
//#define DRAW_TILE_BORDER  //Draws and outline on the tile edges
#define NO_CLIP           //No collision
//#define SPAWN_TESTROOM    //Spawns you in the testroom
#endif

/* |-----------------------------------------------------|
 * |                   TILE INDICES                      |
 * |-----------------------------------------------------|
 */

#define TREE_STUMP
#endif  //MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_
