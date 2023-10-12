#ifndef MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
#define MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_

struct QuestNode {
  std::string objective_text;
  PointI map_marker{0, 0};
  NodeType type;
  bool major_objective = false;
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
  PointI pos;
  int new_tile;
  int layer;
  Zone zone;
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

struct SPAWN final : public QuestNode {
  std::vector<PointI> positions;
  MonsterType type;
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
  std::string txt;
  NPC_ID target;
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
  inline static constexpr int DELAY_TICKS = 250;
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
    if (currentLine == lines.size()) currentLine = 0;
    for (auto npc : NPCS) {
      if (npc->id == target) {
        auto distance = PLAYER.tile_pos.dist(npc->tile_pos);
        if (distance == 0) return true;
        if (distance <= activationDistance) {
          currentTicks += npc->dial_count == 1000;
          if (currentTicks >= DELAY_TICKS) {
            npc->update_dialogue(&lines[currentLine]);
            npc->last = true;
            if (currentLine == lines.size() - 1) currentTicks = -1000;
            currentLine++;
            currentTicks = 0;
          }
        } else {
          currentLine = 0;
          currentTicks = 0;
        }
        return false;
      }
    }
    return false;
  }
};

struct CHOICE_DIALOGUE_SIMPLE final : public QuestNode {
  std::string text;
  std::vector<TexturedButton> choices;
  std::vector<std::string> answers;
  int8_t answerIndex = -1;
  NPC_ID target;
  bool assignedChoices = false;
  explicit CHOICE_DIALOGUE_SIMPLE(NPC_ID target, std::string text)
      : QuestNode("", NodeType::CHOICE_DIALOGUE_SIMPLE),
        target(target),
        text(std::move(text)){};

  bool Progress() noexcept final {
    for (const auto& b : choices) {
      b.UpdateGlobalWindowState();
    }
    if (!assignedChoices) {
      for (const auto npc : NPCS) {
        if (npc->id == target) {
          npc->update_dialogue(&text);
          npc->choices = &choices;
          assignedChoices = true;
          break;
        }
      }
    }

    if (answerIndex != -1) {
      for (const auto npc : NPCS) {
        if (npc->id == target) {
          npc->update_dialogue(&answers[answerIndex]);
          npc->choices = nullptr;
          return true;
        }
      }
    }
    return false;
  }
  inline  void SetAnswerIndex( int num) {
    answerIndex = num;
  }
};
#endif  //MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
