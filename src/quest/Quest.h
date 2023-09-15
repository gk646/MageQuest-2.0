#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "Objectives.h"

struct Quest {
  int stage = 0;
  Quest_ID id;
  std::string name;
  QuestState state;
  std::vector<Objective*> objectives;
  bool hidden;
  Objective* get_current_obj() {
    return objectives[stage];
  }
  void load(const std::string& path) {}
  virtual void finish() = 0;
  void update(){

  }
};

#include "quests/Tutorial.h"

#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
