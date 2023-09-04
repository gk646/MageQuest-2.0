#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_

struct WorldManager {
  static void load_map(Zone zone, Point pos) {
    for (const auto& map : MAPS) {
      if (map.zone == zone) {
        CURRENT_BACK_GROUND = map.map_back_ground;
        CURRENT_MIDDLE_GROUND = map.map_middle_ground;
        CURRENT_FORE_GROUND = map.map_fore_ground;
        CURRENT_ZONE = zone;
        PLAYER.pos = pos * 48;
      }
    }
  }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
