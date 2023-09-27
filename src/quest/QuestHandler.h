#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

#include "Quest.h"

struct QuestHandler {
  Quest* activeQuest = nullptr;
  std::vector<Quest*> quests;
  ~QuestHandler() {
    for (auto q : quests) {
      delete q;
    }
  }
  inline Quest* get_quest(Quest_ID id) {
    for (auto& quest : quests) {
      if (quest->id == id) {
        return quest;
      }
    }
    return nullptr;
  }
  void InteractWithNPC(NPC* npc) {
    for (auto quest : quests) {
      if (quest->progressable(NodeType::SPEAK)) {
        quest->Progress(npc);
      }
    }
  }
  void MonsterKilled(MonsterType type) {
    for (auto quest : quests) {
      if (quest->progressable(NodeType::KILL)) {
        quest->Progress(type);
      }
    }
  }
  void Update() {
    for (auto quest : quests) {
      if (quest->state == QuestState::ACTIVE) {
        quest->Update();
      }
    }
  }
  inline void SetActive(Quest_ID id) noexcept {
    for (auto& quest : quests) {
      if (quest->id == id) {
        activeQuest = quest;
      }
    }
  }
  inline void ClearActive() noexcept { activeQuest = nullptr; }
  [[nodiscard]] inline bool HaveActiveQuest() const noexcept {
    return activeQuest != nullptr;
  }
  [[nodiscard]] inline const std::string& GetObjectiveText() const noexcept {
    if (activeQuest) {
      return activeQuest->GetActiveObjective();
    }
    return "";
  }
  inline void add_quest(Quest* q) noexcept { quests.push_back(q); }
};
inline static QuestHandler PLAYER_QUESTS;

#endif  //MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
