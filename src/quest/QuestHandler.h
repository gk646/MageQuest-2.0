#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

#include "Quest.h"

struct QuestHandler {
  std::vector<Quest*> quests;

  void interact_npc(NPC* npc) {
    Objective* obj;
    for (auto quest : quests) {
      if (quest->state == QuestState::ACTIVE) {
        if ((obj = quest->get_current_obj()) && obj->type == ObjectiveType::SPEAK) {
          ((SPEAK*)obj)->progress(npc);
        }
      }
    }
  }
  void killed_monster() {}
};
inline static QuestHandler PLAYER_QUESTS;

#endif  //MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
