#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

#include "Quest.h"

struct QuestHandler {
  std::vector<Quest*> quests;
  ~QuestHandler() {
    for (auto q : quests) {
      delete q;
    }
  }
  inline Quest* get_quest(Quest_ID id) {
    for (auto quest : quests) {
      if (quest->id == id) {
        return quest;
      }
    }
    return nullptr;
  }
  void interact_npc(NPC* npc) {
    for (auto quest : quests) {
      if (quest->progressable(NodeType::SPEAK)) {
        quest->progress(npc);
      }
    }
  }
  void killed_monster() {}
  void update() {
    for (auto quest : quests) {
      if (quest->state == QuestState::ACTIVE) {
        quest->update();
      }
    }
  }
  inline void add_quest(Quest* q) noexcept { quests.push_back(q); }
};
inline static QuestHandler PLAYER_QUESTS;

#endif  //MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
