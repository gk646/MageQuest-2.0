#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_



struct MapManager {

  static void load_map() {
    CURRENT_BACK_GROUND = MAPS[0].map_back_ground;
    CURRENT_MIDDLE_GROUND = MAPS[0].map_middle_ground;
    CURRENT_FORE_GROUND = MAPS[0].map_fore_ground;
    for (auto& map : MAPS) {}
  }

  static void load_maps() { MAPS.emplace_back("Tutorial", 100); }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
