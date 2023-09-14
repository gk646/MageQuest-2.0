#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "Objective.h"

struct Quest {
  int stage;
  Quest_ID id;
  std::string name;
  QuestState state;
  std::vector<Objective> objectives;
};
#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
