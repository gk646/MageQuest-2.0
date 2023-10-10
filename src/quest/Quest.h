#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "elements/Nodes.h"
#include "elements/QuestReward.h"

struct Quest final {
  std::string name;
  std::vector<std::string> pastObjectives;
  std::vector<std::string> pastDialogue;
  std::vector<QuestNode*> objectives;
  std::string description;
  QuestReward* reward = nullptr;
  int stage = 0;
  Quest_ID id;
  QuestState state = QuestState::IN_ACTIVE;
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
  void Progress(NPC* npc) noexcept {
    if (((SPEAK*)objectives[stage])->progress(npc)) {
      FinishStage(objectives[stage]);
    }
  }
  void Progress(MonsterType type) noexcept {
    if (((KILL*)objectives[stage])->progress(type)) {
      FinishStage(objectives[stage]);
    }
  }
  void Update() {
    if (objectives[stage]->progress()) {
      FinishStage(objectives[stage]);
    }
  }
  [[nodiscard]] inline const std::string& GetActiveObjective() const noexcept {
    return objectives[stage]->objective_text;
  }

 private:
  void CompleteQuest() noexcept { state = QuestState::COMPLETED; }
  inline void FinishStage(const QuestNode* obj) noexcept {
    if (obj->major_objective) {
      //play sound
    }
    stage++;
    if (stage == objectives.size()) {
      CompleteQuest();
    }
  }
};
#include "QuestStorage.h"
#include "ScriptParser.h"
#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
