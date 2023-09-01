#ifndef DUNGEONM_SRC_LOADING_STARTUPLOADER_H_
#define DUNGEONM_SRC_LOADING_STARTUPLOADER_H_

#include "../graphics/MapManager.h"
#include "../graphics/TileManager.h"

struct GameLoader {

  static void load() {
    cxstructs::now();

    TileManager::load_texture_span(0, 5589);
    MapManager::load_maps();
    MapManager::load_map();

    cxstructs::printTime();
  }
};
#endif  //DUNGEONM_SRC_LOADING_STARTUPLOADER_H_
