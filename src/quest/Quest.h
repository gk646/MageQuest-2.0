#ifndef MAGEQUEST_SRC_QUESTS_QUEST_H_
#define MAGEQUEST_SRC_QUESTS_QUEST_H_

#include "ScriptedNodes.h"
#include "elements/Nodes.h"
#include "elements/QuestReward.h"

struct Alternative {
  std::vector<QuestNode*> objectives{};
  int16_t stage = 0;
  [[nodiscard]] inline QuestNode* GetNode() const noexcept { return objectives[stage]; }
  inline void Progress() noexcept { stage++; }
  ~Alternative() {
    for (auto& obj : objectives) {
      if (obj) {
        delete obj;
        obj = nullptr;
      }
    }
  }
};
struct Quest final {
  std::string name;
  std::string description;
  std::vector<std::string> pastDialogue;
  std::vector<QuestNode*> objectives;
  std::vector<Alternative> alternatives{10};
  QuestReward* reward = nullptr;
  int16_t choice = -1;
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
    return state == QuestState::ACTIVE;
  }
  void Progress(NPC* npc) noexcept {
    if (GetCurrentStage()->Progress(npc)) {
      FinishStage(GetCurrentStage());
    }
  }
  void Progress(MonsterType type) noexcept {
    if (((KILL*)GetCurrentStage())->Progress(type)) {
      FinishStage(GetCurrentStage());
    }
  }
  inline void Update() noexcept {
    if (GetCurrentStage()->Progress()) {
      FinishStage(GetCurrentStage());
    }
  }
  [[nodiscard]] inline const std::string& GetActiveObjective() const noexcept {
    return GetCurrentStage()->GetObjectiveText();
  }
  [[nodiscard]] inline PointT<int16_t> GetActiveWaypoint() const noexcept {
    return CURRENT_ZONE == questZone ? GetCurrentStage()->GetWayPoint()
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
  inline static std::string SaveAlternativesStatus(const std::vector<Alternative>& vec) {
    std::string ret{};
    for (const auto& alt : vec) {
      ret.append(std::to_string(alt.stage)).append("|");
    }
    return ret;
  }
  inline static void LoadAlternatives(Quest* quest, const std::string& text) {
    auto nums = Util::SplitString(text, '|');
    for (int i = 0; i < quest->alternatives.size() && i < nums.size(); i++) {
      quest->alternatives[i].stage = std::stoi(nums[i]);
    }
  }

 private:
  [[nodiscard]] inline QuestNode* GetCurrentStage() const noexcept {
    if (choice == -1) {
      return objectives[stage];
    } else {
      return alternatives[choice].GetNode();
    }
  }
  void CompleteQuest() noexcept;
  static void SaveProgress() noexcept;
  inline void FinishStage(const QuestNode* obj) noexcept {
    if (obj->isMajorObjective) {
      SaveProgress();
      PlaySoundR(sound::majorObjective);
    }

    if (obj->type == NodeType::FINISH_QUEST) {
      CompleteQuest();
    }

    if (choice != -1) {
      alternatives[choice].Progress();
    } else {
      stage++;
      if (stage == objectives.size()) {
        CompleteQuest();
      }
    }

    if (obj->type == NodeType::CHOICE_DIALOGUE) {
      choice = ((CHOICE_DIALOGUE*)obj)->choiceNums[((CHOICE_DIALOGUE*)obj)->answerIndex];
    } else if (obj->type == NodeType::SWITCH_ALTERNATIVE) {
      choice = ((SWITCH_ALTERNATIVE*)obj)->choiceNum;
    }
  }
};

#include "QuestStorage.h"
#include "ScriptParser.h"

inline static Quest* GetQuest(Quest_ID id) {
  switch (id) {
    case Quest_ID::TUTORIAL:
      return Quests::TUTORIAL;
    case Quest_ID::MARLA_QUEST:
      return Quests::MARLA_LOST_NECKLACE;
    case Quest_ID::START_SOMETHING_NEW:
      return Quests::START_SOMETHING_NEW;
    case Quest_ID::END:
      return nullptr;
    case Quest_ID::HILLCREST_PUZZLE:
      return Quests::HILLCREST_PUZZLE;
    case Quest_ID::NIETZSCHE_QUEST:
      return Quests::NIETZSCHE;
  }
}

//QuestNode methods using "Quest" class
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
bool CHOICE_DIALOGUE::Progress() noexcept {
  if (!foundNPC) {
    for (const auto npc : NPCS) {
      if (npc->id == target) {
        npcPtr = npc;
        foundNPC = true;
      }
    }
  }
  if (!foundNPC) return false;

  for (auto& b : choices) {
    b.UpdateGlobalWindowState();
  }

  if (!assignedChoices) {
    npcPtr->UpdateDialogue(&questionText);
    TrackText(questionText, TextSource::NPC, (int16_t)target);
    npcPtr->choices = &choices;
    npcPtr->hideContinueButton = true;
    assignedChoices = true;
  }

  if (pickedAnswer && Util::EPressed()) {
    return npcPtr->dialogueProgressCount >= 1000.0F;
  } else if (answerIndex != -1 && !pickedAnswer) {
    npcPtr->UpdateDialogue(&answers[answerIndex]);
    TrackText(questionText, TextSource::NPC, (int16_t)target);
    npcPtr->choices = nullptr;
    npcPtr->hideContinueButton = false;
    pickedAnswer = true;
  }
  return false;
}
bool SWITCH_ALTERNATIVE::Progress() noexcept {
  quest->choice = choiceNum;
  return true;
}
bool SET_QUEST_ZONE::Progress() noexcept {
  quest->questZone = zone;
  return true;
}
bool COMBAT_TRIGGER::Progress() noexcept {
  if (pastChoice == -10) {
    pastChoice = quest->choice;
  }

  if (Finish()) return true;

  auto& obj = objectives[currIndex];
  if (obj->Progress()) {
    if (obj->type == NodeType::CHOICE_DIALOGUE) {
      quest->choice =
          ((CHOICE_DIALOGUE*)obj)->choiceNums[((CHOICE_DIALOGUE*)obj)->answerIndex];
    } else if (obj->type == NodeType::SWITCH_ALTERNATIVE) {
      quest->choice = ((SWITCH_ALTERNATIVE*)obj)->choiceNum;
    }

    currIndex++;
  }
  return false;
}
bool COMBAT_TRIGGER::Finish() noexcept {
  if (currIndex == objectives.size() || PLAYER.isInCombat) {
    quest->choice = pastChoice;
    return true;
  }
}
#endif  //MAGEQUEST_SRC_QUESTS_QUEST_H_
