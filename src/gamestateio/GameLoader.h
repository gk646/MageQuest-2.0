#ifndef MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
#define MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_

#include "loading/loaders/EntityLoader.h"
#include "loading/loaders/QuestLoader.h"
#include "loading/loaders/MapLoader.h"
#include "loading/loaders/SoundLoader.h"
#include "loading/loaders/TextureLoader.h"
#include "loading/loaders/TileLoader.h"
#include "loading/loaders/FontLoader.h"
#include "loading/loaders/EntityStateLoader.h"
#include "loading/loaders/GameInfoLoader.h"

struct GameLoader {
  static std::atomic_bool finished_cpu_loading;
  static std::array<std::function<void()>, 5> load_functions;
  static void load() {
    std::thread worker(load_game);
    worker.detach();
  }
  static void finish_loading() {
    if (finished_cpu_loading &&
        load_util::current_step < load_functions.size() + load_util::cpu_steps) {
      LoadStep(load_functions[load_util::current_step - load_util::cpu_steps]);
#ifdef MG2_DEBUG
    } else if (true) {
#else
    } else if (RAYLIB_LOGO->finished) {
#endif
      SetupGame();
    }
  }

 private:
  static void load_game() {
    LoadStep(GameInfoLoader::Load);
    LoadStep(TransitionParser::ParseTransitionFile);
    LoadStep(NPCLoader::LoadNamedNPCs);
    LoadStep(QuestLoader::load);
    LoadStep(SoundLoader::load);
    LoadStep(TileLoader::load);
    LoadStep(MapLoader::load);
    finished_cpu_loading = true;
  }
  static void SetupGame() {
    PlaySoundR(sound::intro);
    for (uint_fast32_t i = 0; i < 10; i++) {
      if (PLAYER_EQUIPPED[i].item) {
        PLAYER_STATS.equip_item(PLAYER_EQUIPPED[i].item->effects);
      }
    }
    PLAYER_STATS.refill_stats();
#ifdef SPAWN_TESTROOM
    WorldManager::LoadMap(Zone::TestRoom, {24, 34});
#else
    WorldManager::LoadMap(Zone::Hillcrest, {94, 34});
#endif
    GAME_STATE = GameState::MainMenu;
    LoadingScreen::progress = 0;
    finished_cpu_loading = false;
    delete RAYLIB_LOGO;
    PLAYER_QUESTS.AddQuest(Quests::TUTORIAL);
    PLAYER_QUESTS.GetQuest(Quest_ID::TUTORIAL)->state = QuestState::ACTIVE;
    PLAYER_QUESTS.SetAsActiveQuest(Quest_ID::TUTORIAL);
    PLAYER_QUESTS.AddQuest(Quests::MARLA_LOST_NECKLACE);
    PLAYER_QUESTS.GetQuest(Quest_ID::MARLA)->state = QuestState::ACTIVE;
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
std::array<std::function<void()>, 5> GameLoader::load_functions = {
    EntityLoader::load, GuiLoadStyleAshes, TileLoader::load_to_vram, TextureLoader::load,
    DataBaseHandler::load};

#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
