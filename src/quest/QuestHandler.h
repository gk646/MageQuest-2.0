#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

class Quest;

struct QuestHandler {
  std::vector<Quest*> quests;
  Quest* activeQuest = nullptr;
  ~QuestHandler();
  inline Quest* get_quest(Quest_ID id);
  void InteractWithNPC(NPC* npc);
  void MonsterKilled(MonsterType type);
  void Update();
  inline void SetActive(Quest_ID id) noexcept;
  inline void SetShown(Quest_ID id) noexcept;
  [[nodiscard]] inline const std::string& GetObjectiveText() const noexcept;
  inline void ClearActive() noexcept { activeQuest = nullptr; }
  [[nodiscard]] inline bool HasActiveQuest() const noexcept {
    return activeQuest != nullptr;
  }
  inline void AddQuest(Quest* q) noexcept {
    quests.push_back(q);
}
};

inline static QuestHandler PLAYER_QUESTS;

#include "Quest.h"

QuestHandler::~QuestHandler() {
  for (auto q : quests) {
    delete q;
  }
}
Quest* QuestHandler::get_quest(Quest_ID id) {
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
void QuestHandler::SetActive(Quest_ID id) noexcept {
  for (auto& quest : quests) {
    if (quest->id == id && !quest->hidden) {
      activeQuest = quest;
      return;
    }
  }
}
void QuestHandler::SetShown(Quest_ID id) noexcept {
  for (auto& quest : quests) {
    if (quest->id == id) {
      quest->hidden = false;
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
