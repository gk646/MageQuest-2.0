#ifndef MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
#define MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_

struct QuestNode {
  bool major_objective = false;
  PointI waypoint{};
  std::string objective_text;
  NodeType type;
  QuestNode(std::string objective_text, NodeType type)
      : objective_text(std::move(objective_text)), type(type) {}
  [[nodiscard]] inline bool suitable(NodeType event_type) const {
    return event_type == type || event_type == NodeType::MIX;
  };
  virtual bool progress(NPC* npc){};
  virtual bool progress() { return false; };
};

struct GOTO final : public QuestNode {
  PointI target;
  explicit GOTO(std::string objective_text, const PointI& target)
      : QuestNode(std::move(objective_text), NodeType::GOTO), target(target) {}
  inline bool progress() final { return PLAYER.tile_pos == target; }
};
struct NPC_MOVE final : public QuestNode {
  std::vector<PointI> waypoints;
  int waypoint = 0;
  NPC_ID target_id;
  explicit NPC_MOVE(NPC_ID id) : QuestNode("", NodeType::NPC_MOVE), target_id(id) {}
  inline bool progress() final {
    for (auto npc : NPCS) {
      if (npc->id == target_id) {
        if (npc->move_to(waypoints[waypoint])) {
          waypoint++;
          if(waypoint == waypoints.size()){
            return true;
          }
        }
        return false;
      }
    }
    return false;
  }
};
struct SPEAK final : public QuestNode {
  int stage = 0;
  NPC_ID target;
  std::vector<std::string> lines;
  explicit SPEAK(std::string objective_text, NPC_ID target)
      : QuestNode(std::move(objective_text), NodeType::SPEAK), target(target) {}
  bool progress(NPC* npc) final {
    if (npc->id == target) {
      if (stage < lines.size()) {
        npc->update_dialogue(&lines[stage]);
        stage++;
        return false;
      } else {
        npc->dialogue = nullptr;
        return true;
      }
    }
    return false;
  }
};

struct MIX final : public QuestNode {
  std::vector<QuestNode*> objectives;
  explicit MIX(std::string objective_text)
      : QuestNode(std::move(objective_text), NodeType::MIX) {}
};

#endif  //MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
