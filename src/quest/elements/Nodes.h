#ifndef MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
#define MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_

struct QuestNode {
  bool major_objective = false;
  PointI map_marker{0, 0};
  std::string objective_text;
  NodeType type;
  QuestNode(std::string objective_text, NodeType type)
      : objective_text(std::move(objective_text)), type(type) {}
  [[nodiscard]] inline bool suitable(NodeType event_type) const {
    return event_type == type || event_type == NodeType::MIX;
  };
  virtual bool progress() noexcept { return false; };
};

struct GOTO final : public QuestNode {
  PointI target;
  explicit GOTO(std::string obj_txt, const PointI& target)
      : QuestNode(std::move(obj_txt), NodeType::GOTO), target(target) {}
  inline bool progress() noexcept final { return PLAYER.tile_pos == target; }
};
struct NPC_MOVE final : public QuestNode {
  std::vector<PointI> waypoints;
  int waypoint = 0;
  NPC_ID target_id;
  explicit NPC_MOVE(NPC_ID id, std::string obj_txt)
      : QuestNode(std::move(obj_txt), NodeType::NPC_MOVE), target_id(id) {}
  inline bool progress() noexcept final {
    for (auto npc : NPCS) {
      if (npc->id == target_id) {
        if (npc->move_to(waypoints[waypoint])) {
          waypoint++;
          if (waypoint == waypoints.size()) {
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
  explicit SPEAK(std::string obj_txt, NPC_ID target)
      : QuestNode(std::move(obj_txt), NodeType::SPEAK), target(target) {}
  bool progress(NPC* npc) noexcept {
    if (npc->id == target) {
      if (stage < lines.size()) {
        npc->update_dialogue(&lines[stage]);
        if (stage == 0) {
          npc->last = false;
        } else if (stage == lines.size() - 1) {
          npc->last = true;
        }
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
struct KILL final : public QuestNode {
  MonsterType target;
  int amount = 0;
  explicit KILL(MonsterType target, int amount, std::string obj_txt)
      : QuestNode(std::move(obj_txt), NodeType::KILL), amount(amount), target(target) {}
  bool progress() noexcept final { return false; }
  bool progress(MonsterType type) noexcept {
    if (type == target || target == MonsterType::ANY) {
      amount--;
    }
    return amount <= 0;
  }
};
struct TILE_ACTION final : public QuestNode {
  Zone zone;
  PointI pos;
  int new_tile;
  int layer;
  TILE_ACTION(Zone zone, int layer, PointI pos, int new_tile)
      : QuestNode("", NodeType::TILE_ACTION),
        layer(layer),
        pos(pos),
        new_tile(new_tile),
        zone(zone) {}
  bool progress() noexcept final {
    if (CURRENT_ZONE == zone) {
      std::cout << pos.x << std::endl;
      std::cout << pos.y << std::endl;
      if (layer == 0) {
        CURRENT_BACK_GROUND[pos.x][pos.y] = new_tile;
        return true;
      } else if (layer == 1) {
        CURRENT_MIDDLE_GROUND[pos.x][pos.y] = new_tile;
        return true;
      } else if (layer == 2) {
        CURRENT_FORE_GROUND[pos.x][pos.y] = new_tile;
        return true;
      }
    }
    return false;
  }
};
struct MIX final : public QuestNode {
  std::vector<QuestNode*> objectives;
  explicit MIX(std::string obj_txt) : QuestNode(std::move(obj_txt), NodeType::MIX) {}
};
struct SPAWN final : public QuestNode {
  MonsterType type;
  std::vector<PointI> positions;
  int level;
  explicit SPAWN(MonsterType type, int level)
      : QuestNode("", NodeType::SPAWN),
        level(level == 0 ? PLAYER_STATS.level : level),
        type(type) {}
  bool progress() noexcept final {
    for (auto p : positions) {
      MONSTERS.push_back(Monster::GetMonster(p.x * 48, p.y * 48, type, level));
    }
    return true;
  }
};
#endif  //MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
