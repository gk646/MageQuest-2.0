#ifndef MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
#define MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_

#include "Quest.h"

struct QuestHandler {
  std::vector<Quest*> quests;
  Quest* activeQuest = nullptr;
  bool updateHappened = false;
  ~QuestHandler() {
    for (auto& q : quests) {
      if (q) {
        delete q;
        q = nullptr;
      }
    }
  }
  //Called each tick for misc updates
  inline void Update() noexcept {
    
    for (auto it = quests.begin(); it != quests.end();) {
      if ((*it)->state == QuestState::ACTIVE) {
        (*it)->Update();
      }
      ++it;
    }
  }
  //Returns nullptr if questID could not be matched
  inline Quest* GetQuest(Quest_ID id) {
    for (auto& quest : quests) {
      if (quest->id == id) {
        return quest;
      }
    }
    return nullptr;
  }
  //Called when upon interaction with any npc
  void InteractWithNPC(NPC* npc) {
    for (auto quest : quests) {
      if (quest->Progressable(NodeType::SPEAK)) {
        quest->Progress(npc);
      }
    }
  }
  //Called when a monster dies
  inline void MonsterKilled(MonsterType type) {
    for (auto quest : quests) {
      if (quest->Progressable(NodeType::KILL)) {
        quest->Progress(type);
      }
    }
  }
  inline void SetAsActiveQuest(Quest* quest) noexcept {
    if (!quest->hidden && quest->state == QuestState::ACTIVE) {
      activeQuest = quest;
    }
  }
  inline void SetQuestShown(Quest_ID id) noexcept {
    for (auto& quest : quests) {
      if (quest->id == id) {
        quest->hidden = false;
        updateHappened = true;
        return;
      }
    }
  }
  inline void RemoveActiveQuest() noexcept { activeQuest = nullptr; }
  [[nodiscard]] inline bool HasActiveQuest() const noexcept {
    return activeQuest != nullptr;
  }
  inline void AddQuest(Quest* q) noexcept { quests.push_back(q); }
  inline bool IsQuestCompleted(Quest_ID id) noexcept {
    for (const auto& q : quests) {
      if (q->state == QuestState::COMPLETED && q->id == id) {
        return true;
      }
    }
    return false;
  }

 private:
  //DEPRECATED | finished quests are still kept in the vector
  inline void RemoveQuest(Quest* q) noexcept {
    if (activeQuest == q) {
      activeQuest = nullptr;
    }
    delete q;
  }
};

inline static QuestHandler PLAYER_QUESTS;

bool SET_QUEST_SHOWN::Progress() noexcept {
  PLAYER_QUESTS.SetQuestShown(id);
  return true;
}
void Quest::CompleteQuest() noexcept {
  PlaySoundR(sound::completeQuest);
  state = QuestState::COMPLETED;
  GAME_STATISTICS.QuestCompleted(this);
  stage--;
  if (reward) {
    //TODO reward
  }
  PLAYER_QUESTS.updateHappened = true;
  if (PLAYER_QUESTS.activeQuest == this) {
    PLAYER_QUESTS.RemoveActiveQuest();
  }
}

#endif  //MAGEQUEST_SRC_QUEST_QUESTHANDLER_H_
