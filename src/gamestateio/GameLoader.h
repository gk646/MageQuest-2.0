#ifndef MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
#define MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_

#include "loading/loaders/EntityLoader.h"
#include "loading/loaders/MapLoader.h"
#include "loading/loaders/SoundLoader.h"
#include "loading/loaders/TileLoader.h"

struct GameLoader {
  static std::atomic_bool finished_cpu_loading;
  static std::vector<std::function<void()>> load_functions;

  static void load() {
    std::thread worker(load_game);
    worker.detach();
  }

  static void load_game() {
    load_step(SoundLoader::load);
    load_step(load_window_icon);
    load_step(TileLoader::load);
    load_step(MapLoader::load);
    finished_cpu_loading = true;
  }

  static void finish_loading() {
    if (finished_cpu_loading &&
        load_util::current_step < load_functions.size() + load_util::cpu_steps) {
      load_step(load_functions[load_util::current_step - load_util::cpu_steps]);
    } else {
      finished_cpu_loading = true;
      WorldManager::load_map(Zone::Woodland_Edge, {4, 4});
      GAME_STATE = GameState::MainMenu;
      LoadingScreen::progress = 0;
    }
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
std::vector<std::function<void()>> GameLoader::load_functions = {
    EntityLoader::load, GuiLoadStyleCyber, TileLoader::load_to_vram};

#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
