#ifndef MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
#define MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_

#include "loading/loaders/EntityLoader.h"
#include "loading/loaders/MapLoader.h"
#include "loading/loaders/SoundLoader.h"
#include "loading/loaders/TextureLoader.h"
#include "loading/loaders/TileLoader.h"
#include "loading/loaders/FontLoader.h"

struct GameLoader {
  static std::atomic_bool finished_cpu_loading;
  static std::array<std::function<void()>,5> load_functions;

  static void load() {
    std::thread worker(load_game);
    worker.detach();
  }

  static void finish_loading() {
    if (finished_cpu_loading &&
        load_util::current_step < load_functions.size() + load_util::cpu_steps) {
      load_step(load_functions[load_util::current_step - load_util::cpu_steps]);
    } else if (true) {
      PlaySound(sound::intro);
      WorldManager::load_map(Zone::Woodland_Edge, {4, 4});
      GAME_STATE = GameState::MainMenu;
      LoadingScreen::progress = 0;
      finished_cpu_loading = false;
      delete RAYLIB_LOGO;
    }
  }

 private:
  static void load_game() {
    load_step(SoundLoader::load);
    load_step(TileLoader::load);
    load_step(MapLoader::load);
    finished_cpu_loading = true;
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
std::array<std::function<void()>,5> GameLoader::load_functions = {
    EntityLoader::load, GuiLoadStyleAshes, TileLoader::load_to_vram, TextureLoader::load, FontLoader::load};

#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
