#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
namespace WorldManager {
struct TransitionPoint {
  PointI origin;
  PointI dest;
  Zone destZone;
};
std::array<std::vector<TransitionPoint>, (int)Zone::END> zoneTPoints;
}  // namespace WorldManager
#include "elements/TransitionParser.h"
namespace WorldManager {
static void LoadMap(Zone zone, const PointI& pos) {
  for (const auto& map : MAPS) {
    if (map.zone == zone) {
      CURRENT_BACK_GROUND = map.map_back_ground;
      CURRENT_MIDDLE_GROUND = map.map_middle_ground;
      CURRENT_FORE_GROUND = map.map_fore_ground;
      CURRENT_MAP_COVER = map.map_cover;
      CURRENT_MAP_SIZE = map.map_size;
      CURRENT_SPAWN_TRIGGERS = map.spawnTriggers;
      PLAYER.pos = PointI::GetPoint(pos * 48);
      CURRENT_ZONE = zone;
      Lighting::fadeAlpha = 210;
      WorldAnimations::CacheAnimationTiles();
      return;
    }
  }
}
inline static void CheckTransitions() noexcept {
  for (const auto& tPoint : zoneTPoints[(int)CURRENT_ZONE]) {
    if (tPoint.origin == PLAYER.tile_pos) {
      LoadMap(tPoint.destZone, tPoint.dest);
      return;
    }
  }
}
inline static void CheckSpawnTriggers() noexcept {
  if(!CURRENT_SPAWN_TRIGGERS) return;
  for (auto& t : *CURRENT_SPAWN_TRIGGERS) {
    if (t.IsClose()) {
      t.Trigger();
    }
  }
}
inline static void Update() noexcept {
  WorldAnimations::ProgressAnimations();
  CheckSpawnTriggers();
  CheckTransitions();
}
}  // namespace WorldManager
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
