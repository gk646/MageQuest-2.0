#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "elements/Nodes.h"
#include "elements/QuestReward.h"

struct Quest final {
  std::string name;
  std::string description;
  std::vector<std::string> pastDialogue;
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
    delete reward;
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
  //Serialization methods
 public:
  inline static std::vector<std::string> LoadQuestText(const std::string& fullText) {
    return Util::SplitString(fullText, '|');
  }
  inline static std::string SaveQuestText(const std::vector<std::string>& vec) {
    std::string ret;
    for (const auto& qt : vec) {
      ret += qt;
      ret += "|";
    }
    return ret;
  }

 private:
  void CompleteQuest() noexcept;
  static void SaveProgress() noexcept;
  inline void FinishStage(const QuestNode* obj) noexcept {
    if (obj->isMajorObjective) {
      SaveProgress();
      PlaySoundR(sound::majorObjective);
    }
    stage++;
    if (stage == objectives.size()) {
      CompleteQuest();
    }
  }
};
void QuestNode::TrackText(const std::string& s, TextSource source,
                          int16_t enumVal) const noexcept {
  std::string prefix;
  switch (source) {
    case TextSource::NPC:
      prefix = npcIdToStringMap[NPC_ID(enumVal)];
      prefix.append(": ");
      break;
    case TextSource::PLAYER:
      prefix = "You: ";
      break;
    default:
      prefix = "Unknown: ";
      break;
  }

  std::string name;
  name.reserve(prefix.size() + s.size());
  name.assign(prefix).append(s);

  quest->pastDialogue.emplace_back(std::move(name));
}

#include "QuestStorage.h"
#include "ScriptParser.h"

inline static Quest* GetQuest(Quest_ID id) {
  switch (id) {
    case Quest_ID::TUTORIAL:
      return Quests::TUTORIAL;
    case Quest_ID::MARLA:
      return Quests::MARLA_LOST_NECKLACE;
    case Quest_ID::START_SOMETHING_NEW:
      return Quests::START_SOMETHING_NEW;
    case Quest_ID::END:
      return nullptr;
  }
}
#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
