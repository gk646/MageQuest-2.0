#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "elements/Nodes.h"
#include "elements/QuestReward.h"

struct QuestText {
  std::string text;
  int16_t enumVal;
  TextSource source;
};

struct Quest final {
  std::string name;
  std::string description;
  std::vector<QuestText> pastDialogue;
  std::vector<QuestNode*> objectives;
  QuestReward* reward = nullptr;
  int16_t stage = 0;
  Quest_ID id;
  QuestState state = QuestState::ACTIVE;
  bool hidden = false;
  uint8_t questLevel = 0;
  Zone questZone = Zone::TestRoom;
  explicit Quest(Quest_ID id, bool hidden) : id(id), hidden(hidden) {}
  ~Quest() {
    for (auto obj : objectives) {
      delete obj;
    }
    if (reward) {
      delete reward;
    }
  }
  [[nodiscard]] inline bool Progressable(NodeType type) const noexcept {
    return state == QuestState::ACTIVE && objectives[stage]->IsNodeTypeCompatible(type);
  }
  void Progress(NPC* npc) noexcept {
    if (((SPEAK*)objectives[stage])->Progress(npc)) {
      FinishStage(objectives[stage]);
    }
  }
  void Progress(MonsterType type) noexcept {
    if (((KILL*)objectives[stage])->Progress(type)) {
      FinishStage(objectives[stage]);
    }
  }
  inline void Update() noexcept {
    if (objectives[stage]->Progress()) {
      FinishStage(objectives[stage]);
    }
  }
  [[nodiscard]] inline const std::string& GetActiveObjective() const noexcept {
    return objectives[stage]->objectiveText;
  }
  [[nodiscard]] inline PointT<int16_t> GetActiveWaypoint() const noexcept {
    return CURRENT_ZONE == questZone ? objectives[stage]->wayPoint
                                     : PointT<int16_t>{0, 0};
  }

 private:
  void CompleteQuest() noexcept {
    PlaySoundR(sound::completeQuest);
    state = QuestState::COMPLETED;
    GAME_STATISTICS.QuestCompleted(this);
    stage--;
    if (reward) {
      //TODO reward
    }
  }
  void SaveProgress() noexcept {
    //TODO save quest state
  }
  inline void FinishStage(const QuestNode* obj) noexcept {
    if (obj->isMajorObjective) {
      PlaySoundR(sound::majorObjective);
    }
    stage++;
    SaveProgress();
    if (stage == objectives.size()) {
      CompleteQuest();
    }
  }
};
void QuestNode::TrackText(const std::string& s, TextSource source,
                          int16_t enumVal) const noexcept {
  quest->pastDialogue.emplace_back(s, enumVal, source);
}

#include "QuestStorage.h"
#include "ScriptParser.h"
#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
