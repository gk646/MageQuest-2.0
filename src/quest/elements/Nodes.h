#ifndef MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
#define MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
struct Quest;
struct QuestNode {
  std::string objectiveText;
  PointT<int16_t> wayPoint;
  Quest* quest = nullptr;
  NodeType type;
  bool isMajorObjective = false;
  QuestNode(std::string objectiveText, NodeType type, const PointI& wayPoint = {0, 0})
      : objectiveText(std::move(objectiveText)), type(type), wayPoint(wayPoint) {}
  [[nodiscard]] inline bool IsNodeTypeCompatible(NodeType event_type) const {
    return event_type == type || event_type == NodeType::MIX;
  };
  //Main function to progress the node
  virtual bool Progress() noexcept { return false; };
  //Adds an entry to the quests past dialogues with the corresponding source
  inline void TrackText(const std::string& s, TextSource source,
                        int16_t enumVal) const noexcept;
};
struct GOTO final : public QuestNode {
  PointI target;
  Zone zone;
  explicit GOTO(std::string obj_txt, const PointI& target, Zone zone)
      : QuestNode(std::move(obj_txt), NodeType::GOTO, target),
        target(target),
        zone(zone) {}
  inline bool Progress() noexcept final {
    return CURRENT_ZONE == zone && PLAYER.tilePos.dist(target) < 3;
  }
  static GOTO* ParseQuestNode(const std::vector<std::string>& parts) noexcept {
    return new GOTO(parts[2], Util::ParsePointI(parts[1]), stringToZoneMap[parts[3]]);
  }
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
  static NPC_MOVE* ParseQuestNode(const std::vector<std::string>& parts) noexcept {
    auto obj = new NPC_MOVE(stringToNPCID[parts[1]], parts[2]);
    obj->wayPoint = {std::stoi(Util::SplitString(parts[3], ',')[1]),
                     std::stoi(Util::SplitString(parts[3], ',')[2])};
    for (int i = 4; i < parts.size(); i++) {
      auto vec = Util::SplitString(parts[i], ',');
      obj->waypoints.emplace_back(std::stoi(vec[0]), std::stoi(vec[1]));
    }
    return obj;
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
        npc->UpdateDialogue(&lines[stage]);
        TrackText(lines[stage], TextSource::NPC, (int16_t)target);
        if (stage == 0) {
          npc->hideContinueButton = false;
        }
        stage++;
        return false;
      } else {
        npc->dialogue = nullptr;
        npc->hideContinueButton = false;
        return true;
      }
    }
    return false;
  }
  inline static SPEAK* ParseQuestNode(const std::vector<std::string>& parts) {
    auto obj = new SPEAK(parts[2], stringToNPCID[parts[1]]);
    if (parts.size() > 3) {
      obj->wayPoint = {std::stoi(Util::SplitString(parts[3], ',')[1]),
                       std::stoi(Util::SplitString(parts[3], ',')[2])};
    }
    return obj;
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
  inline static KILL* ParseQuestNode(const std::vector<std::string>& parts) {
    return new KILL(stringToMonsterID[Util::SplitString(parts[1], ',')[0]],
                    std::stoi(Util::SplitString(parts[1], ',')[1]), parts[2]);
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
      if (layer == 0) {
        CURRENT_BACK_GROUND[pos.x][pos.y] = (int16_t)new_tile;
        return true;
      } else if (layer == 1) {
        CURRENT_MIDDLE_GROUND[pos.x][pos.y] = (int16_t)new_tile;
        return true;
      } else if (layer == 2) {
        CURRENT_FORE_GROUND[pos.x][pos.y] = (int16_t)new_tile;
        return true;
      }
    }
    return false;
  }
  inline static TILE_ACTION* ParseQuestNode(const std::vector<std::string>& parts) {
    int layer = -1;
    if (parts[2] == "BACK") {
      layer = 0;
    } else if (parts[2] == "MIDDLE") {
      layer = 1;
    } else if (parts[2] == "FORE") {
      layer = 2;
    }
    return new TILE_ACTION(stringToZoneMap[parts[1]], layer, Util::ParsePointI(parts[3]),
                           std::stoi(parts[4]));
  }
};
/**
 * @brief SPAWN Command Documentation
 *
 * Use SPAWN to instantiate game entities at various locations.
 *
 * Syntax:
 * @code
 *     SPAWN:ENTITY_TYPE:LEVEL:POINT[:POINT]...
 * @endcode
 *
 * - ENTITY_TYPE: Entity to be spawned (e.g., "WOLF").
 * - LEVEL: Entity's level as an integer.
 * - POINT: X,Y coordinates. Can specify multiple using colons.
 *
 * Example:
 * @code
 *     SPAWN:WOLF:1:42,30:42,37:44,34
 * @endcode
 *
 * @note No spaces in the command.
 */
struct SPAWN final : public QuestNode {
  std::vector<PointT<int16_t>> positions;
  MonsterType mType = MonsterType::ANY;
  NPC_ID npcID = NPC_ID::NPC_END;
  ItemType type;
  int id;
  Zone zone = CURRENT_ZONE;
  int level;
  explicit SPAWN(MonsterType type, int level)
      : QuestNode("", NodeType::SPAWN),
        level(level == 0 ? PLAYER_STATS.level : level),
        mType(type) {}
  explicit SPAWN(NPC_ID npcID, int level)
      : QuestNode("", NodeType::SPAWN),
        level(level == 0 ? PLAYER_STATS.level : level),
        npcID(npcID) {}
  bool Progress() noexcept final {
    if (mType != MonsterType::ANY) {
      for (const auto& p : positions) {
        MONSTERS.push_back(Monster::GetNewMonster(
            {(float)p.x * 48.0F, (float)p.y * 48.0F}, mType, level, zone));
      }
      return true;
    } else if (npcID != NPC_ID::NPC_END) {
      for (const auto& p : positions) {
        NPCS.push_back(
            NPC::GetNewNPC(npcID, (float)p.x * 48.0F, (float)p.y * 48.0F, zone));
      }
      return true;
    } else if (type != ItemType::EMPTY) {
      auto item =
          new DroppedItem({positions[0].x * 48.0F, positions[0].y * 48.0F},
                          Item::FindBaseItemClone(id, type, 85, PLAYER_STATS.level));
      item->currentZone = zone;
      WORLD_OBJECTS.emplace_back(item);
    } else {
      return false;
    }
  }
  inline static SPAWN* ParseQuestNode(const std::vector<std::string>& parts) {
    SPAWN* obj;
    if (stringToMonsterID.contains(parts[1])) {
      obj = new SPAWN(stringToMonsterID[parts[1]], std::stoi(parts[2]));
    } else if (stringToNPCID.contains(parts[1])) {
      obj = new SPAWN(stringToNPCID[parts[1]], std::stoi(parts[2]));
    } else {
      obj = new SPAWN(NPC_ID::NPC_END, std::stoi(parts[2]));
      auto idVec = Util::SplitString(parts[1], ',');
      obj->type = ItemType((uint8_t)std::stoi(idVec[0]));
      obj->id = std::stoi(idVec[1]);
    }
    for (int i = 3; i < parts.size(); i++) {
      obj->positions.emplace_back(Util::ParsePointI(parts[i]));
    }
    auto zoneString = Util::SplitString(parts[2], ',');
    if (zoneString.size() > 1) {
      if (stringToZoneMap.contains(zoneString[1])) {
        obj->zone = stringToZoneMap[zoneString[1]];
      }
    }
    return obj;
  }
};
/**

    @brief NPC_SAY Command Documentation

    Use NPC_SAY to make a specific NPC say a dialogue text.
    Syntax:
    @code

    NPC_SAY:TARGET_ID[:SKIP]

@endcode

    TARGET_ID: The ID of the NPC that will speak.

    SKIP (optional): Skips waiting for the dialogue to finish.

Example:
@code
NPC_SAY:GUARD

NPC_SAY:SHOPKEEPER:SKIP

@endcode
@note No spaces in the command.
*/
struct NPC_SAY final : public QuestNode {
  int currLine = 0;
  const int waitTicks = 90;
  int upCounter = 0;
  std::vector<std::string> lines;
  NPC_ID target;
  bool skipWait = false;
  bool startedTalking = false;
  explicit NPC_SAY(NPC_ID target) : QuestNode("", NodeType::NPC_SAY), target(target) {}
  bool Progress() noexcept final {
    for (auto npc : NPCS) {
      if (npc->id == target) {
        if (!startedTalking) {
          npc->hideContinueButton = true;
          npc->UpdateDialogue(&lines[currLine]);
          TrackText(lines[currLine], TextSource::NPC, (int16_t)target);
          startedTalking = true;
        } else if (npc->dialogueProgressCount >= 1000.0F || skipWait) {
          upCounter++;
          if (upCounter == waitTicks) {
            upCounter = 0;
            currLine++;
            startedTalking = false;
          }
          if (currLine == lines.size()) return true;
        }
        return false;
      }
    }
    return false;
  }

  inline static NPC_SAY* ParseQuestNode(const std::vector<std::string>& parts) {
    auto obj = new NPC_SAY(stringToNPCID[parts[1]]);
    if (parts.size() > 2) {
      if (parts[2] == "SKIP") {
        obj->skipWait = true;
      }
    }
    return obj;
  }
};
struct NPC_SAY_PROXIMITY final : public QuestNode {
  inline static constexpr int DELAY_TICKS = 350;
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
        if (npc->currentZone != CURRENT_ZONE) return false;
        auto distance = (int)PLAYER.tilePos.dist(npc->tilePos);
        if (distance == 0) return true;
        if (distance <= activationDistance) {
          currentTicks += npc->dialogueProgressCount == 1000;
          if (currentTicks >= DELAY_TICKS) {
            npc->UpdateDialogue(&lines[currentLine]);
            npc->hideContinueButton = true;
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
  inline static NPC_SAY_PROXIMITY* ParseQuestNode(const std::vector<std::string>& parts) {
    return new NPC_SAY_PROXIMITY(stringToNPCID[parts[1]], std::stoi(parts[2]));
  }
};
struct CHOICE_DIALOGUE_SIMPLE final : public QuestNode {
  std::string questionText;
  std::vector<TexturedButton> choices;
  std::vector<std::string> answers;
  int16_t upCounter = 0;
  int8_t answerIndex = -1;
  NPC_ID target;
  bool assignedChoices = false;
  uint8_t correctAnswer;
  NPC* npcPtr = nullptr;
  bool foundNPC = false;
  bool pickedAnswer = false;
  explicit CHOICE_DIALOGUE_SIMPLE(NPC_ID target, std::string text, int correctAnswer)
      : QuestNode("Speak with " + npcIdToStringMap[target],
                  NodeType::CHOICE_DIALOGUE_SIMPLE),
        target(target),
        questionText(std::move(text)),
        correctAnswer(correctAnswer){};
  bool Progress() noexcept final {
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

    if (pickedAnswer) {
      if (npcPtr->dialogueProgressCount == 1000.0F && Util::EPressed()) {
        if (answerIndex == correctAnswer) {
          return true;
        } else {
          pickedAnswer = false;
          answerIndex = -1;
        }
      }
    } else if (answerIndex != -1) {
      npcPtr->UpdateDialogue(&answers[answerIndex]);
      TrackText(questionText, TextSource::NPC, (int16_t)target);
      npcPtr->choices = nullptr;
      npcPtr->hideContinueButton = false;
      pickedAnswer = true;
    } else if (npcPtr->dialogueProgressCount == 1000) {
      if (npcPtr->dialogue != &questionText) {
        if (npcPtr->dialogueProgressCount == 1000) {
          npcPtr->UpdateDialogue(&questionText);
          npcPtr->dialogueProgressCount = 1000;
          npcPtr->choices = &choices;
          npcPtr->hideContinueButton = true;
        }
      }
    }
    return false;
  }
  inline void SetAnswerIndex(int num) { answerIndex = (int8_t)num; }
  inline static CHOICE_DIALOGUE_SIMPLE* ParseQuestNode(
      const std::vector<std::string>& parts) {
    return new CHOICE_DIALOGUE_SIMPLE(stringToNPCID[parts[1]], parts[2],
                                      std::stoi(parts[3]));
  }
};
struct SET_QUEST_SHOWN final : public QuestNode {
  Quest_ID id;
  explicit SET_QUEST_SHOWN(Quest_ID id)
      : QuestNode("", NodeType::SET_QUEST_SHOWN), id(id) {}
  bool Progress() noexcept final;
};
struct SET_QUEST_ZONE final : public QuestNode {
  Zone zone;
  explicit SET_QUEST_ZONE(const std::string& zoneString)
      : QuestNode("", NodeType::SET_QUEST_ZONE), zone(stringToZoneMap[zoneString]) {}
  bool Progress() noexcept final;
};
struct PLAYER_THOUGHT final : public QuestNode {
  std::string thought;
  float count = 0;
  bool assigned = false;
  PLAYER_THOUGHT(std::string thought, std::string objective)
      : QuestNode(std::move(objective), NodeType::PLAYER_THOUGHT),
        thought(std::move(thought)) {}
  bool Progress() noexcept final {
    if (!assigned) {
      TextRenderer::playerText = &thought;
      TrackText(thought, TextSource::PLAYER, 0);
      TextRenderer::playerDialogueCount = &count;
      assigned = true;
    }
    return count > 100;
  }
  inline static PLAYER_THOUGHT* ParseQuestNode(const std::vector<std::string>& parts) {
    return new PLAYER_THOUGHT(parts[1], parts[2]);
  }
};
struct PointOptional {
  QuestNode* node = nullptr;
  PointT<int16_t> point;
  bool isEssential = false;
  bool isFinished = false;
};
struct OPTIONAL_POSITION final : public QuestNode {
  std::vector<PointOptional> choices;
  int limit;
  explicit OPTIONAL_POSITION(const std::vector<std::string>& parts)
      : QuestNode(parts[1], NodeType::OPTIONAL_POSITION), limit((int)parts.size() - 2) {
    for (int i = 2; i < parts.size(); i++) {
      PointOptional opt;
      opt.point = Util::ParsePointI(parts[i]);
      opt.isEssential = std::stoi(Util::SplitString(parts[i], ',')[2]) == 1;
      choices.emplace_back(opt);
    }
  }
  bool Progress() noexcept final {
    bool allFinished = true;
    for (auto& c : choices) {
      if (c.isEssential && !c.isFinished) {
        allFinished = false;
      }
      //maybe adjust that behaviour
      //either keep calling if close once
      //or just manually set to true like now
      if (c.isFinished || c.point.dist(PLAYER.tilePos) > 1.4F) continue;

      c.node->Progress();
      c.isFinished = true;
    }

    return allFinished;
  }
};
struct SKIP final : public QuestNode {
  SKIP() : QuestNode("", NodeType::SKIP) {}
  bool Progress() noexcept final { return true; }
};
struct COLLECT final : public QuestNode {
  int id;
  ItemType type;
  COLLECT(const std::string& itemID, const std::string& objective)
      : QuestNode(objective, NodeType::COLLECT) {
    auto idVec = Util::SplitString(itemID, ',');
    type = ItemType(std::stoi(idVec[0]));
    id = std::stoi(idVec[1]);
  }
  bool Progress() noexcept final {
    int bagSlots = (int)PLAYER_STATS.GetBagSlots();
    for (int i = 0; i < bagSlots; i++) {
      auto item = PLAYER_BAG[i].item;
      if (item && item->id == id && item->type == type) {
        return true;
      }
    }
    return false;
  }
  inline static COLLECT* ParseQuestNode(const std::vector<std::string>& parts) {
    return new COLLECT(parts[1], parts[2]);
  }
};
struct REMOVE_ITEM final : public QuestNode {
  int id;
  ItemType type;
  int quantity = 0;
  REMOVE_ITEM(const std::string& itemID, const std::string& quantityString)
      : QuestNode("", NodeType::REMOVE_ITEM) {
    auto idVec = Util::SplitString(itemID, ',');
    type = ItemType(std::stoi(idVec[0]));
    id = std::stoi(idVec[1]);
    quantity = std::stoi(quantityString);
  }
  bool Progress() noexcept final {
    int bagSlots = (int)PLAYER_STATS.GetBagSlots();
    for (int i = 0; i < bagSlots; i++) {
      auto item = PLAYER_BAG[i].item;
      if (item && item->id == id && item->type == type && quantity > 0) {
        delete PLAYER_BAG[i].item;
        PLAYER_BAG[i].item = nullptr;
        quantity--;
      }
    }
    return quantity == 0;
  }
  inline static REMOVE_ITEM* ParseQuestNode(const std::vector<std::string>& parts) {
    return new REMOVE_ITEM(parts[1], parts[2]);
  }
};
struct WAIT final : public QuestNode {
  int ticks = 0;
  explicit WAIT(const std::string& timeString)
      : QuestNode("", NodeType::WAIT), ticks(std::stoi(timeString)) {}
  bool Progress() noexcept final {
    if (ticks == 0) return true;
    ticks--;
  }
  inline static WAIT* ParseQuestNode(const std::vector<std::string>& parts) {
    return new WAIT(parts[1]);
  }
};
struct CHOICE_DIALOGUE final : public QuestNode {
  std::string questionText;
  std::vector<TexturedButton> choices;
  std::vector<std::string> answers;
  NPC* npcPtr = nullptr;
  int16_t choiceNums[5] = {-1, -1, -1, -1, -1};
  int8_t answerIndex = -1;
  NPC_ID target;
  bool foundNPC = false;
  bool assignedChoices = false;
  bool pickedAnswer = false;
  explicit CHOICE_DIALOGUE(NPC_ID target, std::string text)
      : QuestNode("Speak with " + npcIdToStringMap[target], NodeType::CHOICE_DIALOGUE),
        target(target),
        questionText(std::move(text)) {}
  bool Progress() noexcept final;
  inline void SetAnswerIndex(int num) { answerIndex = (int8_t)num; }
  inline static CHOICE_DIALOGUE* ParseQuestNode(const std::vector<std::string>& parts) {
    return new CHOICE_DIALOGUE(stringToNPCID[parts[1]], parts[2]);
  }
};
struct SWITCH_ALTERNATIVE final : public QuestNode {
  int16_t choiceNum;
  explicit SWITCH_ALTERNATIVE(const std::string& text)
      : QuestNode("", NodeType::SWITCH_ALTERNATIVE), choiceNum(std::stoi(text)) {}
  bool Progress() noexcept final;
  inline static SWITCH_ALTERNATIVE* ParseQuestNode(
      const std::vector<std::string>& parts) {
    return new SWITCH_ALTERNATIVE(parts[1]);
  }
};
struct SET_NPC_POS final : public QuestNode {
  NPC_ID target;
  Point pos;
  Zone zone = CURRENT_ZONE;
  explicit SET_NPC_POS(const std::vector<std::string>& vec)
      : QuestNode("", NodeType::SET_NPC_POS),
        target(stringToNPCID[vec[1]]),
        pos(Util::ParsePoint(vec[2])),
        zone(stringToZoneMap[vec[3]]) {
    pos = pos * 48;
  }
  bool Progress() noexcept final {
    for (auto& npc : NPCS) {
      if (npc->id == target) {
        npc->pos = pos;
        npc->currentZone = zone;
        return true;
      }
    }
    return true;
  }
};
struct FINISH_QUEST final : public QuestNode {
  FINISH_QUEST() : QuestNode("", NodeType::FINISH_QUEST) {}
  bool Progress() noexcept final { return true; }
};
/**
 * Attempts to remove 1 (the first) of the given npc type
 */
struct DESPAWN_NPC final : public QuestNode {
  NPC_ID target;
  explicit DESPAWN_NPC(const std::string& targetString)
      : QuestNode("", NodeType::DESPAWN_NPC), target(stringToNPCID[targetString]) {}
  bool Progress() noexcept final {
    for (auto& npc : NPCS) {
      if (npc->id == target) {
        npc->isDead = true;
        return true;
      }
    }
    return true;
  }
};

#endif  //MAGEQUEST_SRC_QUESTS_OBJECTIVE_H_
