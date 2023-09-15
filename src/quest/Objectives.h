#ifndef MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
#define MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_

struct Objective {
  bool complete = false;
  std::string objective_text;
  ObjectiveType type;
  Objective(std::string objective_text, ObjectiveType type)
      : objective_text(std::move(objective_text)), type(type) {}

};

struct GOTO final : public Objective {

  explicit GOTO(std::string objective_text) : Objective(std::move(objective_text),ObjectiveType::GOTO) {}

  void update() {

  }
};
struct SPEAK final : public Objective {
  int stage = 0;
  NPC_ID target;
  std::vector<std::string> lines;
  explicit SPEAK(std::string objective_text) : Objective(std::move(objective_text),ObjectiveType::SPEAK) {}

  void progress(NPC* npc) {
    if(npc->id == target){
      npc->
          dialogue = &lines[stage];
      stage++;
    }
  }
};
#endif  //MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
