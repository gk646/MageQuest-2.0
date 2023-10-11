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
  virtual bool Progress() noexcept { return false; };
};

struct GOTO final : public QuestNode {
  PointI target;
  explicit GOTO(std::string obj_txt, const PointI& target)
      : QuestNode(std::move(obj_txt), NodeType::GOTO), target(target) {}
  inline bool Progress() noexcept final { return PLAYER.tile_pos == target; }
};
struct NPC_MOVE final : public QuestNode {
  std::vector<PointI> waypoints;
  int waypoint = 0;
  NPC_ID target_id;
  explicit NPC_MOVE(NPC_ID id, std::string obj_txt)
      : QuestNode(std::move(obj_txt), NodeType::NPC_MOVE), target_id(id) {}
  inline bool Progress() noexcept final {
    for (auto npc : NPCS) {
      if (npc->id == target_id) {
        if (npc->MoveToPointI(waypoints[waypoint])) {
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
  bool Progress(NPC* npc) noexcept {
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
  bool Progress() noexcept final { return false; }
  bool Progress(MonsterType type) noexcept {
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
  TILE_ACTION(Zone zone, int layer, const PointI& pos, int new_tile)
      : QuestNode("", NodeType::TILE_ACTION),
        layer(layer),
        pos(pos),
        new_tile(new_tile),
        zone(zone) {}
  bool Progress() noexcept final {
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
  bool Progress() noexcept final {
    for (const auto& p : positions) {
      MONSTERS.push_back(Monster::GetMonster(p.x * 48, p.y * 48, type, level));
    }
    return true;
  }
};
struct NPC_SAY final : public QuestNode {
  NPC_ID target;
  std::string txt;
  bool skipWait = false;
  bool startedTalking = false;
  explicit NPC_SAY(NPC_ID target) : QuestNode("", NodeType::MIX), target(target) {}
  bool Progress() noexcept final {
    for (auto npc : NPCS) {
      if (npc->id == target) {
        if (!startedTalking) {
          npc->update_dialogue(&txt);
          startedTalking = true;
        } else if (npc->dial_count == 1000 || skipWait) {
          return true;
        }
        return false;
      }
    }
    return false;
  }
};
struct NPC_SAY_PROXIMITY final : public QuestNode {
  inline static constexpr int DELAY_TICKS = 150;
  NPC_ID target;
  std::vector<std::string> lines;
  int activationDistance = 0;
  int currentLine = 0;
  int currentTicks = DELAY_TICKS;
  NPC_SAY_PROXIMITY(NPC_ID target, int distance)
      : QuestNode("", NodeType::NPC_SAY_PROXIMITY),
        target(target),
        activationDistance(distance) {}
  bool Progress() noexcept final {
    for (auto npc : NPCS) {
      if (npc->id == target) {
        auto distance = PLAYER.tile_pos.dist(npc->tile_pos);
        if (distance <= activationDistance) {
          currentTicks++;
          if (distance == 0) return true;
          if (currentTicks >= DELAY_TICKS && npc->dial_count == 1000) {
            if (currentLine == lines.size() - 1) return true;
            npc->update_dialogue(&lines[currentLine]);
            currentLine++;
            currentTicks = 0;
          }
        }
      }
    }
    return false;
  }
};
#endif  //MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
