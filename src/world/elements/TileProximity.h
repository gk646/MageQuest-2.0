#ifndef MAGEQUEST_SRC_WORLD_TILEPROXIMITY_H_
#define MAGEQUEST_SRC_WORLD_TILEPROXIMITY_H_
namespace TileProximity {
inline static constexpr int UPDATE_TICKS = 30;
inline static constexpr int RADIUS = 10;
inline static uint16_t currentTick = 0;
inline static bool isWaterClose = false;
inline static bool isFireplaceClose = false;
inline static bool isTorchClose = false;
inline static void IsRangeInProximity(bool& assign, int16_t tilIndexStart,
                                      int16_t tilIndexEnd, int16_t backG,
                                      int16_t middleG) noexcept {
  if (!assign && ((backG >= tilIndexStart && backG <= tilIndexEnd) ||
                  (middleG >= tilIndexStart && middleG <= tilIndexEnd))) {
    assign = true;
  }
}
inline static void ScanProximityTiles() noexcept {
  auto pos = PLAYER.tilePos;
  int sx = pos.x;
  int sy = pos.y;
  int radiusSqrt = RADIUS * RADIUS;
  int xMin = std::max(sx - RADIUS, 0);
  int xMax = std::min(sx + RADIUS, CURRENT_MAP_SIZE);
  int yMin = std::max(sy - RADIUS, 0);
  int yMax = std::min(sy + RADIUS, CURRENT_MAP_SIZE);
  for (int x = xMin; x < xMax; ++x) {
    int dx = (x - sx) * (x - sx);
    for (int y = yMin; y < yMax; ++y) {
      if (dx + (y - sy) * (y - sy) <= radiusSqrt) {
        auto num = CURRENT_BACK_GROUND[x][y];
        auto num2 = CURRENT_MIDDLE_GROUND[x][y];
        IsRangeInProximity(isWaterClose, 910, 1035, num, num2);
        IsRangeInProximity(isWaterClose, 8398, 8458, num, num2);
        IsRangeInProximity(isFireplaceClose, 1528, 1531, num, num2);
        IsRangeInProximity(isTorchClose, 1404, 1411, num, num2);
      }
    }
  }
}
inline static void UpdateProximitySounds() noexcept {
  if (isWaterClose) {
    MusicStreamer::StartPlaylist(&sound::music::waterAmbience);
  } else {
    MusicStreamer::StopPlaylist(&sound::music::waterAmbience);
  }

  if (isFireplaceClose) {
    MusicStreamer::AddTrack(&sound::music::firePlace);
  } else {
    MusicStreamer::RemoveTrack(&sound::music::firePlace);
  }
}
inline static void ResetState() noexcept {
  isWaterClose = false;
  isFireplaceClose = false;
  isTorchClose = false;
}
inline static void Update() noexcept {
  currentTick++;
  if (currentTick != UPDATE_TICKS) return;
  ResetState();
  ScanProximityTiles();
  UpdateProximitySounds();
  currentTick = 0;
}
}  // namespace TileProximity
#endif  //MAGEQUEST_SRC_WORLD_TILEPROXIMITY_H_
