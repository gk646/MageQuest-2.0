#ifndef MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_
#define MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_

/* |-----------------------------------------------------|
 * |                      SYSTEM                         |
 * |-----------------------------------------------------|
 */
#define SIMD_PRAGMA _Pragma("omp simd")
#define TILE_SIZE 48
#define SCALE(val) ((val)*UI_SCALE)
#define SCALE_RECT(rect) \
  { SCALE((rect).x), SCALE((rect).y), SCALE((rect).width), SCALE((rect).height) }

/* |-----------------------------------------------------|
 * |                    MULTIPLAYER                      |
 * |-----------------------------------------------------|
 */
#define MG2_NET_IDENTIFIER "MageQuest"
#define MG2_PORT 42042
#define MG2_TICK_RATE 60
#define MG2_SERVER_BUSY 42
#define MG2_TICK_TIME 0.0166

/* |-----------------------------------------------------|
 * |                TILE COLLISIONS                      |
 * |-----------------------------------------------------|
 */
#define C_SOLID 1
#define C_AIR_ONLY 2
#define C_WATER 3

//Directions
#define LEFT 0
#define RIGHT 1
#define DOWN 2
#define UP 3

//UI
#define SLOT_SIZE 45

/* |-----------------------------------------------------|
 * |                       DEBUG                         |
 * |-----------------------------------------------------|
 */
//#define DRAW_HITBOXES                                                                   //Draws an outline around hitboxes
//#define DRAW_TILE_BORDER                                                                //Draws and outline on the tile edges

#endif  //MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_
