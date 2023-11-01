#ifndef MAGEQUEST_SRC_WORLD_TILEEVENTS_H_
#define MAGEQUEST_SRC_WORLD_TILEEVENTS_H_

namespace TileEvents {
//active tiles / BG / BG1 / FG
inline static int16_t ACTIVE_TILES[3];
}  // namespace TileEvents
#include "elements/TileActions.h"
#include "elements/TileProximity.h"
#include "elements/TileEffects.h"

namespace TileEvents {

inline void Update() noexcept {
  TileProximity::Update();
  auto pos = PLAYER.tilePos;
  ACTIVE_TILES[0] = CURRENT_BACK_GROUND[pos.x][pos.y];
  ACTIVE_TILES[1] = CURRENT_MIDDLE_GROUND[pos.x][pos.y];
  ACTIVE_TILES[2] = CURRENT_FORE_GROUND[pos.x][pos.y];
}
}  // namespace TileEvents

#endif  //MAGEQUEST_SRC_WORLD_TILEEVENTS_H_
