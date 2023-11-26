#ifndef MAGEQUEST_SRC_QUEST_QUESTREWARD_H_
#define MAGEQUEST_SRC_QUEST_QUESTREWARD_H_
struct QuestReward {
  int16_t experience;
  int32_t goldCoins;
  inline static void Activate(QuestReward* reward) noexcept {
    if (!reward) return;
    PLAYER_SECOND_STATS.AddPlayerExperience(reward->experience);
    PLAYER_SECOND_STATS.AddCoins(reward->goldCoins);
  }
};
#endif  //MAGEQUEST_SRC_QUEST_QUESTREWARD_H_
