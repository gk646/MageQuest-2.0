#ifndef MAGEQUEST_SRC_GAMEPLAY_GAMESTATISTICS_H_
#define MAGEQUEST_SRC_GAMEPLAY_GAMESTATISTICS_H_
struct Quest;
struct GameStatistics {
  float statistics[GAME_STATISTICS_END] = {};
  inline void Update() noexcept {
    if (GAME_STATE == GameState::Game || GAME_STATE == GameState::GameMenu) {
      statistics[GAME_TICKS_PLAYED]++;
    }
  }
  inline void WalkPixels(float pixels) noexcept { statistics[PIXELS_WALKED] += pixels; }
  inline void MonsterKilled(const MonsterType type) noexcept {
    statistics[MONSTERS_KILLED]++;
    if (IsBoss(type)) {
      statistics[BOSSES_KILLED]++;
    }
  }
  inline void PickedUpItem(const ItemRarity rarity) noexcept { statistics[ITEMS_PICKED_UP]++; }
  inline void QuestCompleted(const Quest* quest) noexcept { statistics[QUESTS_COMPLETED]++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_GAMESTATISTICS_H_
