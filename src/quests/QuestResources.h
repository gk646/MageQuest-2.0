#ifndef MAGEQUEST_SRC_QUESTS_QUESTRESOURCES_H_
#define MAGEQUEST_SRC_QUESTS_QUESTRESOURCES_H_

struct Dialogue {
  int stage;
  std::string text;
};
struct Quest_Dialogue {
  Quest_ID id;
  std::vector<Dialogue> lines;
  inline const std::string& get_dialogue(int quest_stage) const {
    for (const auto& dial : lines) {
      if (quest_stage == dial.stage) {
        return dial.text;
      }
    }
    return lines[0].text;
  }
  inline const std::string& get_random() const{
    std::uniform_int_distribution<> distr(0, lines.size());
    return lines[distr(RNG_ENGINE)].text;
  }
};
namespace QuestResources {
std::array<std::vector<Quest_Dialogue>, (int)NPC_ID::END> dialog_storage;

inline static const std::string& get_quest_dialogue(const NPC_ID id) noexcept {
  if(id == NPC_ID::VILLAGER){

  }else if (id == NPC_ID::TRADER){

  }else if( id == NPC_ID::RANDOM){

  }

  for (const auto& quests_d : dialog_storage[(int)id]) {
    for (const auto& quest : PLAYER_QUESTS) {
      if (quest.id == quests_d.id) {
        return quests_d.get_dialogue(quest.stage);
      }
    }
  }
  return dialog_storage[(int) NPC_ID::RANDOM][0].get_random();
}
static void load_quests() {}
}  // namespace QuestResources
#endif  //MAGEQUEST_SRC_QUESTS_QUESTRESOURCES_H_
