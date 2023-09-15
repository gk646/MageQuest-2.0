#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

#include "Quest.h"

struct QuestHandler {
  std::vector<Quest*> quests;

  void interact_npc(NPC* npc) {
    for (auto quest : quests) {
      if (quest->progressable(NodeType::SPEAK)) {
        quest->progress(npc);
      }
    }
  }
  void killed_monster() {}
  void update_void() {}
  inline void add_quest(Quest* q) noexcept {

    quests.push_back(q);

  }
  ~QuestHandler() {
    for (auto q : quests) {
      delete q;
    }
  }
};
inline static QuestHandler PLAYER_QUESTS;

#endif  //MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
