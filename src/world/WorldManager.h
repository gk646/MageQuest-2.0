#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
struct TransitionPoint {
  PointT<int16_t> origin;
  PointT<int16_t> dest;
  Zone destZone;
};
namespace WorldManager {
std::array<std::vector<TransitionPoint>, (int)Zone::ZONE_END> zoneTPoints;
}  // namespace WorldManager
#include "elements/TransitionParser.h"
namespace WorldManager {
//Teleports player into the new map at position "pos * 48"
static void LoadMap(Zone zone, const PointT<int16_t>& pos) {
  for (const auto& map : MAPS) {
    if (map.zone == zone) {
      CURRENT_BACK_GROUND = map.mapBackGround;
      CURRENT_MIDDLE_GROUND = map.mapMiddleGround;
      CURRENT_FORE_GROUND = map.mapForeGround;
      CURRENT_MAP_COVER = map.mapCover;
      CURRENT_MAP_SIZE = map.mapSize;
      CURRENT_SPAWN_TRIGGERS = map.spawnTriggers;
      CURRENT_ZONE = zone;
      PROJECTILES.clear();
      PLAYER.pos = PointT<int16_t>::GetPoint(pos * 48);
      Lighting::fadeAlpha = 253;
      WorldAnimations::CacheAnimationTiles();
      Lighting::AmbientOcclusion::GenerateShadowMap();
      return;
    }
  }
}
//Check if the player is inside a teleport tile
inline static void CheckTransitions() noexcept {
  for (const auto& tPoint : zoneTPoints[(int)CURRENT_ZONE]) {
    if (tPoint.origin == PLAYER.tilePos) {
      LoadMap(tPoint.destZone, tPoint.dest);
      return;
    }
  }
}
//Iterate all spawn triggers and trigger them on proximity
inline static void CheckSpawnTriggers() noexcept {
  if (!CURRENT_SPAWN_TRIGGERS) return;
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
