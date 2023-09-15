#ifndef MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
#define MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
namespace ScriptParser {
std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

Quest* load(const std::string& path, Quest_ID u_id) {
  auto quest = new Quest();
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

  while (std::getline(file, line)) {
    if (line.empty()) continue;
    std::vector<std::string> parts;
    parts = split(line, ':');
    type = node_to_type[parts[0]];
    switch (type) {
      case NodeType::GOTO: {
        auto obj = new GOTO(parts[2], {std::stoi(split(parts[1], ',')[0]),
                                       std::stoi(split(parts[1], ',')[1])});
        obj->major_objective = parts.size() > 3;
        quest->objectives.push_back(obj);

      } break;
      case NodeType::KILL:
        break;
      case NodeType::SPEAK: {
        auto obj = new SPEAK(parts[2], npcIdMap[parts[1]]);
        std::getline(file, line);
        assert(line == "*");
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
      case NodeType::NPC_MOVE:
        auto obj = new NPC_MOVE(npcIdMap[parts[1]]);
        for (uint_fast32_t i = 2; i < parts.size(); i++) {
          auto vec = split(parts[i], ',');
          obj->waypoints.emplace_back(std::stoi(vec[0]), std::stoi(vec[1]));
        }
        break;
    }
  }

  return quest;
}

QuestNode* parse_SPEAK(const std::vector<std::string>&) {}
};      // namespace ScriptParser
#endif  //MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
