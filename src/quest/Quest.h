#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "elements/Nodes.h"
#include "elements/QuestReward.h"

struct Quest final {
  int stage = 0;
  Quest_ID id;
  QuestReward* reward = nullptr;
  std::string name;
  QuestState state = QuestState::IN_ACTIVE;
  std::vector<QuestNode*> objectives;
  bool hidden = false;
  explicit Quest(Quest_ID id) : id(id) {}
  ~Quest() {
    for (auto obj : objectives) {
      delete obj;
    }
    delete reward;
  }
  [[nodiscard]] inline bool progressable(NodeType type) const noexcept {
    return state == QuestState::ACTIVE && objectives[stage]->suitable(type);
  }
  void progress(NPC* npc) {
    if (objectives[stage]->progress(npc)) {
      finish_stage(objectives[stage]);
    }
  }
  void update() {
    if (objectives[stage]->progress()) {
      finish_stage(objectives[stage]);
    }
  }
  inline const std::string& get_objective_text() const noexcept {
    return objectives[stage]->objective_text;
  }

 private:
  void complete_quest() noexcept { state = QuestState::COMPLETED; }
  inline void finish_stage(const QuestNode* obj) noexcept {
    if (obj->major_objective) {
      //play sound
    }
    stage++;
    if (stage == objectives.size()) {
      complete_quest();
    }
  }
};
#include "ScriptParser.h"
#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
