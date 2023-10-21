#ifndef MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_
#define MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_

namespace TransitionParser {

static void ParseTransitionFile() {
  auto table = Util::ReadMGI("mgi/TransitionMap.mgi");

  Zone zone;
  for (const auto& i : table) {
    zone = stringToZoneMap[i[0]];
    PointI dest = Util::ParsePointI(i[1]);
    Zone destZone = stringToZoneMap[i[2]];
    for (uint_fast32_t j = 3; j < i.size(); j++) {
      WorldManager::zoneTPoints[(int)zone].emplace_back(Util::ParsePointI(i[j]), dest,
                                                        destZone);
    }
  }
}
}  // namespace TransitionParser
#endif  //MAGEQUEST_SRC_WORLD_TRANSITIONPARSER_H_
