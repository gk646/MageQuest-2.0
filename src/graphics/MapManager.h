#ifndef DUNGEONM_SRC_GRAPHICS_MAPMANAGER_H_
#define DUNGEONM_SRC_GRAPHICS_MAPMANAGER_H_

#include "maps/Map.h"

struct MapManager {

  static void load_map() {
    CURRENT_BACK_GROUND = MAPS[0].map_back_ground;
    CURRENT_MIDDLE_GROUND = MAPS[0].map_middle_ground;
    CURRENT_FORE_GROUND = MAPS[0].map_fore_ground;
    std::cout<< CURRENT_BACK_GROUND[0][0] << std::endl;
    for (auto& map : MAPS) {}
  }

  static void load_maps() { MAPS.emplace_back("Tutorial", 100); }
};
#endif  //DUNGEONM_SRC_GRAPHICS_MAPMANAGER_H_
