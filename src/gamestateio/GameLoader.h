#ifndef MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
#define MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_

#include "../graphics/MapManager.h"
#include "loaders/TileLoader.h"

struct GameLoader {
  static std::atomic_bool finished_cpu_loading;

  static void load() {
    std::thread worker(load_game);
    worker.detach();
  }

  static void load_game() {

    TileLoader::load();
    MapManager::load_maps();
    MapManager::load_map();
    finished_cpu_loading = true;

  }

  static void finish_loading() {
    if (finished_cpu_loading) {
      TileLoader::load_to_vram();
      GAME_STATE = GameState::MainMenu;
    }
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
