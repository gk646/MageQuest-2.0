#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

class Quest;

struct QuestHandler {
  std::vector<Quest*> quests;
  Quest* activeQuest = nullptr;
  bool updateHappened = false;
  ~QuestHandler();
  /**
   * Returns nullptr if questID could not be matched
   * @param questID
   * @return a ptr to the quest or null
   */
  inline Quest* GetQuest(Quest_ID questID);
  void InteractWithNPC(NPC* npc);
  void MonsterKilled(MonsterType type);
  void Update();
  inline void SetAsActiveQuest(Quest_ID id) noexcept;
  inline void SetQuestShown(Quest_ID id) noexcept;
  [[nodiscard]] inline const std::string& GetObjectiveText() const noexcept;
  inline void RemoveActiveQuest() noexcept { activeQuest = nullptr; }
  [[nodiscard]] inline bool HasActiveQuest() const noexcept {
    return activeQuest != nullptr;
  }
  inline void AddQuest(Quest* q) noexcept { quests.push_back(q); }
};

inline static QuestHandler PLAYER_QUESTS;

#include "Quest.h"

QuestHandler::~QuestHandler() {
  for (auto q : quests) {
    delete q;
  }
}
Quest* QuestHandler::GetQuest(Quest_ID id) {
  for (auto& quest : quests) {
    if (quest->id == id) {
      return quest;
    }
  }
  return nullptr;
}
void QuestHandler::InteractWithNPC(NPC* npc) {
  for (auto quest : quests) {
    if (quest->progressable(NodeType::SPEAK)) {
      quest->Progress(npc);
    }
  }
}
void QuestHandler::MonsterKilled(MonsterType type) {
  for (auto quest : quests) {
    if (quest->progressable(NodeType::KILL)) {
      quest->Progress(type);
    }
  }
}
void QuestHandler::Update() {
  for (auto quest : quests) {
    if (quest->state == QuestState::ACTIVE) {
      quest->Update();
    }
  }
}
void QuestHandler::SetAsActiveQuest(Quest_ID id) noexcept {
  for (auto& quest : quests) {
    if (quest->id == id && !quest->hidden) {
      activeQuest = quest;
      return;
    }
  }
}
void QuestHandler::SetQuestShown(Quest_ID id) noexcept {
  for (auto& quest : quests) {
    if (quest->id == id) {
      quest->hidden = false;
      updateHappened = true;
      return;
    }
  }
}
const std::string& QuestHandler::GetObjectiveText() const noexcept {
  if (activeQuest) {
    return activeQuest->GetActiveObjective();
  }
  return "";
}
#endif  //MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
