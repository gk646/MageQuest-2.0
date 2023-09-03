#ifndef MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
#define MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_

struct GameLoader {
  static std::atomic_bool finished_cpu_loading;
  static void load() {
    std::thread worker(load_game);
    worker.detach();
  }

  static void load_game() {
    load_step(SoundLoader::load);
    load_step(load_window_icon);
    load_step(TileLoader::load);
    load_step(MapManager::load_maps);
    load_step(MapManager::load_map);
    finished_cpu_loading = true;
  }

  static void finish_loading() {
    if (finished_cpu_loading) {
      load_step(GuiLoadStyleCyber);
      load_step(TileLoader::load_to_vram);

      //PlaySound(sound::intro);
      GAME_STATE = GameState::MainMenu;
      LoadingScreen::progress = 0;
      finished_cpu_loading = false;
    }
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
