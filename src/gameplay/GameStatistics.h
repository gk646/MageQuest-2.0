#ifndef MAGEQUEST_SRC_GAMEPLAY_GAMESTATISTICS_H_
#define MAGEQUEST_SRC_GAMEPLAY_GAMESTATISTICS_H_
struct GameStatistics {
  float statistics[GameStatistic::END] = {};

  inline void Update() noexcept {
    if (GAME_STATE == GameState::Game) {
      statistics[GAME_TICKS_PLAYED]++;
    }
  }
  inline void WalkPixels(float pixels) noexcept { statistics[PIXELS_WALKED] += pixels; }

  inline void MonsterKilled(MonsterType type) noexcept { statistics[MONSTERS_KILLED]++; }
  inline void PickedUpItem(ItemRarity rarity) noexcept { statistics[ITEMS_PICKED_UP]++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_GAMESTATISTICS_H_
