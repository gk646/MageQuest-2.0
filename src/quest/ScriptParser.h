#ifndef MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
#define MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
namespace ScriptParser {

Quest* load(const std::string& path, Quest_ID id) {
  auto quest = new Quest(id);
  std::ifstream file(ASSET_PATH + path);
  NodeType type;
  std::string line;
  if (!file.is_open()) {
    std::cerr << "Failed to open quest file." << std::endl;
    return nullptr;
  }

  std::getline(file, line);
  quest->name = split(line, ':')[1];
  std::getline(file, line);
  std::getline(file, line);
  std::vector<std::string> parts;
  while (std::getline(file, line)) {
    if (line.empty()) continue;
    parts.clear();
    parts = split(line, ':');
    type = node_to_type[parts[0]];
    switch (type) {
      case NodeType::GOTO: {
        auto obj = new GOTO(parts[2], ParsePointI(parts[1]));
        obj->major_objective = parts[parts.size() - 1] == "MAJOR";
        quest->objectives.push_back(obj);

      } break;
      case NodeType::KILL: {
        auto obj = new KILL(stringToMonsterID[split(parts[1], ',')[0]],
                            std::stoi(split(parts[1], ',')[1]), parts[2]);
        obj->major_objective = parts[parts.size() - 1] == "MAJOR";
        quest->objectives.push_back(obj);
      } break;
      case NodeType::SPEAK: {
        auto obj = new SPEAK(parts[2], npcIdMap[parts[1]]);
        if (parts.size() > 3) {
          obj->map_marker = {std::stoi(split(parts[3], ',')[1]),
                             std::stoi(split(parts[3], ',')[2])};
        }
        obj->major_objective = parts[parts.size() - 1] == "MAJOR";
        while (std::getline(file, line) && line != "*") {
          obj->lines.push_back(line);
        }
        quest->objectives.push_back(obj);
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
        obj->map_marker = {std::stoi(split(parts[3], ',')[1]),
                           std::stoi(split(parts[3], ',')[2])};
        for (uint_fast32_t i = 4; i < parts.size(); i++) {
          auto vec = split(parts[i], ',');
          obj->waypoints.emplace_back(std::stoi(vec[0]), std::stoi(vec[1]));
        }
        quest->objectives.push_back(obj);

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
                                   ParsePointI(parts[3]), std::stoi(parts[4]));
        quest->objectives.push_back(obj);
      } break;
      case NodeType::NPC_SAY: {
        auto obj = new NPC_SAY(npcIdMap[parts[1]]);
        if (parts.size() > 2) {
          if (parts[2] == "TRUE") {
            obj->blockingUntilLineFinished = true;
          }
        }
        std::getline(file, obj->txt);
        std::getline(file, line);
        quest->objectives.push_back(obj);
      } break;
      case NodeType::SPAWN: {
        auto obj = new SPAWN(stringToMonsterID[parts[1]], std::stoi(parts[2]));
        for (uint_fast32_t i = 3; i < parts.size(); i++) {
          obj->positions.emplace_back(ParsePointI(parts[i]));
        }
        quest->objectives.push_back(obj);
      } break;
      default:
        break;
    }
  }
  return quest;
}
}  // namespace ScriptParser
#endif  //MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
