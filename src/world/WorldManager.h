#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_

struct WorldManager {
  static void load_map(Zone zone, Point pos) {
    for (const auto& map : MAPS) {
      if (map.zone == zone) {
        GAME_STATE = GameState::Loading;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        CURRENT_BACK_GROUND = map.map_back_ground;
        CURRENT_MIDDLE_GROUND = map.map_middle_ground;
        CURRENT_FORE_GROUND = map.map_fore_ground;
        CURRENT_MAP_COVER = map.map_cover;
        CURRENT_ZONE = zone;
        WorldAnimations::CacheAnimationTiles();
        PLAYER.pos = pos * 48;
        ScreenEffects::fadeAlpha = 254;
        GAME_STATE = GameState::Game;
        return;
      }
    }
  }

};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPMANAGER_H_
