#ifndef MAGEQUEST_SRC_WORLD_TILEPROXIMITY_H_
#define MAGEQUEST_SRC_WORLD_TILEPROXIMITY_H_
namespace TileProximity {
std::unordered_set<int16_t> proximityTiles;
inline static constexpr int UPDATE_TICKS = 30;
inline static constexpr int RADIUS = 10;
inline static uint16_t currentTick = 0;
inline static void CacheProximityTiles() noexcept {
  auto pos = PLAYER.tilePos;
  int sx = pos.x;
  int sy = pos.y;
  int radiusSqrt = RADIUS * RADIUS;
  int xMin = std::max(sx - RADIUS, 0);
  int xMax = std::min(sx + RADIUS, CURRENT_MAP_SIZE);
  int yMin = std::max(sy - RADIUS, 0);
  int yMax = std::min(sy + RADIUS, CURRENT_MAP_SIZE);
  proximityTiles.clear();
  for (int x = xMin; x < xMax; ++x) {
    int dx = (x - sx) * (x - sx);
    for (int y = yMin; y < yMax; ++y) {
      if (dx + (y - sy) * (y - sy) <= radiusSqrt) {
        proximityTiles.insert(CURRENT_BACK_GROUND[x][y]);
        proximityTiles.insert(CURRENT_MIDDLE_GROUND[x][y]);
      }
    }
  }
}

inline static void Update() noexcept {
  currentTick++;
  if (currentTick != UPDATE_TICKS) return;
  CacheProximityTiles();
  currentTick = 0;
}
}  // namespace TileProximity
#endif  //MAGEQUEST_SRC_WORLD_TILEPROXIMITY_H_
