#ifndef MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_
#define MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_

namespace TransitionParser {

static void ParseTransitionFile() {
  std::array<std::vector<WorldManager::TransitionPoint>, (int)Zone::END> tempPoints;
  std::ifstream file(ASSET_PATH + "Maps/TransitionMap.mgt");

  if (!file.is_open()) {
    std::cerr << "Failed to open quest file." << std::endl;
  }
  Zone zone;
  std::string line;
  std::vector<std::string> parts;
  while (std::getline(file, line)) {
    if (line.empty()) break;
    parts.clear();
    parts = Util::SplitString(line, ':');
    zone = stringToZoneMap[parts[0]];
    PointI dest = Util::ParsePointI(parts[1]);
    Zone destZone = stringToZoneMap[parts[2]];
    for (uint_fast32_t i = 3; i < parts.size(); i++) {
      tempPoints[(int)zone].emplace_back(Util::ParsePointI(parts[i]),dest,destZone);
    }
  }

  WorldManager::zoneTPoints = tempPoints;
}
}  // namespace TransitionParser
#endif  //MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_
