#ifndef MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
#define MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
namespace ScriptParser {

#define ADD_TO_QUEST()                                        \
  obj->isMajorObjective = parts[parts.size() - 1] == "MAJOR"; \
  quest->objectives.push_back(obj);

Quest* load(const std::string& path, Quest_ID id, bool hidden = false) {
  auto quest = new Quest(id, hidden);
  std::ifstream file(ASSET_PATH + path);
  NodeType type;
  std::string line;
  if (!file.is_open()) {
    std::cerr << "Failed to open quest file." << std::endl;
    return nullptr;
  }

  std::getline(file, line);
  quest->name = Util::SplitString(line, ':')[1];
  std::getline(file, line);
  std::getline(file, line);
  quest->description = Util::SplitString(line, ':')[1];
  std::vector<std::string> parts;
  while (std::getline(file, line)) {
    if (line.empty() || line.starts_with('#')) continue;
    parts.clear();
    parts = Util::SplitString(line, ':');
    type = node_to_type[parts[0]];
    switch (type) {
      case NodeType::PLAYER_THOUGHT: {
        auto obj = PLAYER_THOUGHT::ParseQuestNode(parts);
        ADD_TO_QUEST();
      } break;
      case NodeType::GOTO: {
        auto obj = GOTO::ParseQuestNode(parts);
        ADD_TO_QUEST();
      } break;
      case NodeType::KILL: {
        auto obj = new KILL(stringToMonsterID[Util::SplitString(parts[1], ',')[0]],
                            std::stoi(Util::SplitString(parts[1], ',')[1]), parts[2]);
        ADD_TO_QUEST();
      } break;
      case NodeType::SPEAK: {
        auto obj = new SPEAK(parts[2], npcIdMap[parts[1]]);
        if (parts.size() > 3) {
          obj->wayPoint = {std::stoi(Util::SplitString(parts[3], ',')[1]),
                           std::stoi(Util::SplitString(parts[3], ',')[2])};
        }
        while (std::getline(file, line) && line != "*") {
          obj->lines.push_back(line);
        }
        ADD_TO_QUEST();
      } break;
      case NodeType::COLLECT:
        break;
      case NodeType::REQUIRE:
        break;
      case NodeType::PROTECT:
        break;
      case NodeType::ESCORT:
        break;
      case NodeType::MIX:
        break;
      case NodeType::NPC_MOVE: {
        auto obj = new NPC_MOVE(npcIdMap[parts[1]], parts[2]);
        obj->wayPoint = {std::stoi(Util::SplitString(parts[3], ',')[1]),
                         std::stoi(Util::SplitString(parts[3], ',')[2])};
        for (uint_fast32_t i = 4; i < parts.size(); i++) {
          auto vec = Util::SplitString(parts[i], ',');
          obj->waypoints.emplace_back(std::stoi(vec[0]), std::stoi(vec[1]));
        }
        ADD_TO_QUEST();

      } break;
      case NodeType::TILE_ACTION: {
        int layer = -1;
        if (parts[2] == "BACK") {
          layer = 0;
        } else if (parts[2] == "MIDDLE") {
          layer = 1;
        } else if (parts[2] == "FORE") {
          layer = 2;
        }
        auto obj = new TILE_ACTION(stringToZoneMap[parts[1]], layer,
                                   Util::ParsePointI(parts[3]), std::stoi(parts[4]));
        ADD_TO_QUEST();
      } break;
      case NodeType::NPC_SAY: {
        auto obj = new NPC_SAY(npcIdMap[parts[1]]);
        if (parts.size() > 2) {
          if (parts[2] == "SKIP") {
            obj->skipWait = true;
          }
        }
        std::getline(file, obj->txt);
        std::getline(file, line);
        ADD_TO_QUEST();
      } break;
      case NodeType::SPAWN: {
        SPAWN* obj;
        if (stringToMonsterID.contains(parts[1])) {
          obj = new SPAWN(stringToMonsterID[parts[1]], std::stoi(parts[2]));
        } else {
          obj = new SPAWN(npcIdMap[parts[1]], std::stoi(parts[2]));
        }
        for (uint_fast32_t i = 3; i < parts.size(); i++) {
          obj->positions.emplace_back(Util::ParsePointI(parts[i]));
        }
        ADD_TO_QUEST();
      } break;
      case NodeType::NPC_SAY_PROXIMITY: {
        auto obj = new NPC_SAY_PROXIMITY(npcIdMap[parts[1]], std::stoi(parts[2]));
        while (std::getline(file, line) && line != "*") {
          obj->lines.push_back(line);
        }
        ADD_TO_QUEST();
      } break;
      case NodeType::CHOICE_DIALOGUE_SIMPLE: {
        auto* obj =
            new CHOICE_DIALOGUE_SIMPLE(npcIdMap[parts[1]], parts[2], std::stoi(parts[3]));
        int i = 0;
        while (std::getline(file, line) && line != "*") {
          auto choice = Util::SplitString(line, ':');
          auto textBound = MeasureTextEx(MINECRAFT_REGULAR, choice[0].c_str(), 16, 0.5F);
          auto boundFunction = [obj, i] {
            obj->SetAnswerIndex(i);
          };
          obj->choices.emplace_back(
              textBound.x + 20, 20, choice[0], 16, textures::ui::questpanel::choiceBox,
              textures::ui::questpanel::choiceBoxHovered,
              textures::ui::questpanel::choiceBoxHovered, 255, "", boundFunction);
          obj->answers.emplace_back(choice[1]);
          i++;
        }
        ADD_TO_QUEST();
      } break;
      case NodeType::SET_QUEST_SHOWN: {
        auto obj = new SET_QUEST_SHOWN(stringToQuestID[parts[1]]);
        ADD_TO_QUEST();
      } break;
      default:
        break;
    }
  }
  return quest;
}
}  // namespace ScriptParser
#endif  //MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
