#ifndef MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_
#define MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_

#define TILE_SIZE 48

//SYSTEM
#define SIMD_PRAGMA _Pragma("omp simd")

//Tile Collision
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

#define SCALE(val) ((val)*UI_SCALE)
#define SCALE_RECT(rect) \
  { SCALE((rect).x), SCALE((rect).y), SCALE((rect).width), SCALE((rect).height) }
#define SCALE_TEXT_SIZE(size) (SCALE(size))


//GAMEPLAY
#define EFFECT_COUNT 50

//#define DRAW_HITBOXES

#endif  //MAGE_QUEST_SRC_SYSTEM_DEFINITIONS_H_
