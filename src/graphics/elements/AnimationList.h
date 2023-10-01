#ifndef MAGEQUEST_SRC_GRAPHICS_ELEMENTS_ANIMATIONLIST_H_
#define MAGEQUEST_SRC_GRAPHICS_ELEMENTS_ANIMATIONLIST_H_
struct AnimatedTile {
  PointI tile;
  uint16_t counter;
  MapLayer layer;
  uint8_t currentIndex;
};

struct AnimationList {
  const std::vector<uint16_t> tileNums;
  uint8_t speed;
  std::vector<AnimatedTile> animatedTiles{};
  AnimationList(std::vector<uint16_t> tileNums, uint8_t len, uint8_t speed)
      : tileNums(std::move(tileNums)), speed(speed) {
    animatedTiles.reserve(5);
  }

  inline void ProgressTiles() noexcept {
    for (auto& tile : animatedTiles) {
      if (tile.counter > speed) {
        tile.counter = 0;
        tile.currentIndex++;
        if (tile.currentIndex == tileNums.size()) {
          tile.currentIndex = 0;
        }
        switch (tile.layer) {
          case MapLayer::BACK_GROUND:
            CURRENT_BACK_GROUND[tile.tile.x][tile.tile.y] = tileNums[tile.currentIndex];
            break;
          case MapLayer::MIDDLE_GROUND:
            CURRENT_MIDDLE_GROUND[tile.tile.x][tile.tile.y] = tileNums[tile.currentIndex];
            break;
          case MapLayer::FRONT:
            CURRENT_FORE_GROUND[tile.tile.x][tile.tile.y] = tileNums[tile.currentIndex];
            break;
        }
      }
      tile.counter++;
    }
  }
  [[nodiscard]] inline bool Contains(uint16_t tileNum) const noexcept {
    for(auto num : tileNums){
      if(num == tileNum){
        return true;
      }
    }
    return false;
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_ELEMENTS_ANIMATIONLIST_H_
