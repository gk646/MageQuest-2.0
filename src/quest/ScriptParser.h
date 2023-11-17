#ifndef MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
#define MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
namespace ScriptParser {
//Parses and returns a ptr to the next node found in the current line - "parts"
inline QuestNode* ParseNextNode(const std::vector<std::string>& parts,
                                std::ifstream& file) noexcept {
  auto type = node_to_type[parts[0]];
  switch (type) {
    case NodeType::SKIP:
      return new SKIP();
    case NodeType::OPTIONAL_POSITION: {
      auto obj = new OPTIONAL_POSITION(parts);
      std::string line;
      int i = 0;
      while (std::getline(file, line) && line != "*" && i < obj->limit) {
        obj->choices[i].node = ParseNextNode(Util::SplitString(line, ':'), file);
        i++;
      }
      return obj;
    }
    case NodeType::PLAYER_THOUGHT:
      return PLAYER_THOUGHT::ParseQuestNode(parts);
    case NodeType::GOTO:
      return GOTO::ParseQuestNode(parts);
    case NodeType::KILL:
      return KILL::ParseQuestNode(parts);
    case NodeType::SPEAK: {
      std::string line;
      auto obj = SPEAK::ParseQuestNode(parts);
      while (std::getline(file, line) && line != "*") {
        obj->lines.push_back(line);
      }
      return obj;
    }
    case NodeType::COLLECT:
    case NodeType::REQUIRE:
    case NodeType::PROTECT:
    case NodeType::ESCORT:
    case NodeType::MIX:
      break;
    case NodeType::NPC_MOVE:
      return NPC_MOVE::ParseQuestNode(parts);
    case NodeType::TILE_ACTION:
      return TILE_ACTION::ParseQuestNode(parts);
    case NodeType::NPC_SAY: {
      std::string line;
      auto obj = NPC_SAY::ParseQuestNode(parts);
      std::getline(file, obj->txt);
      std::getline(file, line);
      return obj;
    }
    case NodeType::SPAWN:
      return SPAWN::ParseQuestNode(parts);
    case NodeType::NPC_SAY_PROXIMITY: {
      std::string line;
      auto obj = NPC_SAY_PROXIMITY::ParseQuestNode(parts);
      while (std::getline(file, line) && line != "*") {
        obj->lines.push_back(line);
      }
      return obj;
    }
    case NodeType::CHOICE_DIALOGUE_SIMPLE: {
      std::string line;
      auto* obj = CHOICE_DIALOGUE_SIMPLE::ParseQuestNode(parts);
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
      return obj;
    }
    case NodeType::SET_QUEST_SHOWN:
      return new SET_QUEST_SHOWN(stringToQuestID[parts[1]]);
    default:
      return nullptr;
  }
}
//Adds the node to the given quest and setting the "isMajor" variable
inline void AddToQuest(QuestNode* node, Quest* q,
                       const std::vector<std::string>& parts) noexcept {
  node->quest = q;
  node->isMajorObjective = parts[parts.size() - 1] == "MAJOR";
  q->objectives.push_back(node);
}
//Parses a ".mgqs" file and returns a ptr to a new instance of this quest
Quest* load(const std::string& path, Quest_ID id, bool hidden = false) {
  auto quest = new Quest(id, hidden);
  std::ifstream file(ASSET_PATH + path);
  std::string line;
  if (!file.is_open()) {
    std::cerr << "Failed to open quest file." << std::endl;
    return nullptr;
  }

  std::getline(file, line);
  quest->name = Util::SplitString(line, ':')[1];
  std::getline(file, line);
  //TODO reward
  std::getline(file, line);
  quest->description = Util::SplitString(line, ':')[1];
  std::getline(file, line);
  quest->questZone = stringToZoneMap[Util::SplitString(line, ':')[1]];
  std::getline(file, line);
  quest->questLevel = std::stoi(Util::SplitString(line, ':')[1]);

  std::vector<std::string> parts;
  while (std::getline(file, line)) {
    if (line.empty() || line.starts_with('#')) continue;
    parts.clear();
    parts = Util::SplitString(line, ':');
    auto node = ParseNextNode(parts, file);
    AddToQuest(node, quest, parts);
  }
  return quest;
}

}  // namespace ScriptParser
#endif  //MAGEQUEST_SRC_QUEST_SCRIPTPARSER_H_
