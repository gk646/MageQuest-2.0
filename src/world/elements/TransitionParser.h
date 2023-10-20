#ifndef MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_
#define MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_

namespace TransitionParser {

static void ParseTransitionFile() {
  auto table = Util::ReadMGI("Maps/TransitionMap.mgi");

  Zone zone;
  for (uint_fast32_t i = 0; i < table.size(); i++) {
    zone = stringToZoneMap[table[i][0]];
    PointI dest = Util::ParsePointI(table[i][1]);
    Zone destZone = stringToZoneMap[table[i][2]];
    for (uint_fast32_t j = 3; j < table[i].size(); j++) {
      WorldManager::zoneTPoints[(int)zone].emplace_back(Util::ParsePointI(table[i][j]),
                                                        dest, destZone);
    }
  }
}
}  // namespace TransitionParser
#endif  //MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_
