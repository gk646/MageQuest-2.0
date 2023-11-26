#ifndef MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
#define MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_

#include "loading/loaders/EntityLoader.h"
#include "loading/loaders/QuestLoader.h"
#include "loading/loaders/StatLoader.h"
#include "loading/loaders/MapLoader.h"
#include "loading/loaders/MiscLoader.h"
#include "loading/loaders/SoundLoader.h"
#include "loading/loaders/TextureLoader.h"
#include "loading/loaders/TileLoader.h"
#include "loading/loaders/FontLoader.h"
#include "loading/loaders/EntityStateLoader.h"
#include "loading/loaders/GameInfoLoader.h"
#include "loading/loaders/SkillLoader.h"
#include "loading/loaders/MusicLoader.h"
#include "loading/loaders/ItemLoader.h"
#include "loading/loaders/TalentLoader.h"
#include "loading/loaders/ProjectileLoader.h"

struct GameLoader {
  static std::atomic_bool finished_cpu_loading;
  static std::array<std::function<void()>, 6> load_functions;
  static void Load() {
    std::thread worker(LoadGame);
    worker.detach();
  }
  static void LoadWithGPU() {
    if (finished_cpu_loading &&
        load_util::current_step < load_functions.size() + load_util::cpu_steps) {
      LoadStep(load_functions[load_util::current_step - load_util::cpu_steps]);
#ifdef MG2_DEBUG
    } else if (true) {
#else
    } else if (RAYLIB_LOGO->isFinished) {
#endif
      SetupGame();
    }
  }

 private:
  static void LoadGame() {
    LoadStep(MiscLoader::Load);
    LoadStep(ProjectileLoader::Load);
    LoadStep(TalentLoader::LoadConnections);
    LoadStep(MusicLoader::Load);
    LoadStep(SkillLoader::Load);
    LoadStep(GameInfoLoader::Load);
    LoadStep(TransitionParser::ParseTransitionFile);
    LoadStep(NPCLoader::LoadNamedNPCs);
    LoadStep(QuestLoader::Load);
    LoadStep(SoundLoader::Load);
    LoadStep(TileLoader::Load);
    LoadStep(MapLoader::Load);

    finished_cpu_loading = true;
  }
  static void SetupGameImpl() {
    StatLoader::Load();
    delete RAYLIB_LOGO;
    PlaySoundR(sound::intro);
    PLAYER_STATS.RefillStats();
    GAME_STATE = GameState::MainMenu;
    LoadingScreen::progress = 0;
    finished_cpu_loading = false;
  }
  static void SetupGame() {
    SetupGameImpl();
#ifdef SPAWN_TESTROOM
    WorldManager::LoadMap(Zone::TestRoom, {24, 34});
#else
    WorldManager::LoadMap(Zone::Hillcrest, {91, 45});
#endif

   // MONSTERS.push_back(        Monster::GetNewMonster({50 * 48, 35 * 48}, MonsterType::GOBLIN, 1));
    // MONSTERS.push_back(        Monster::GetNewMonster({50 * 48, 35 * 48}, MonsterType::SKULL_WOLF, 5));
    for (int i = 0; i < 15; i++) {
      // WORLD_OBJECTS.push_back(          new DroppedItem({50 * 48, 35 * 48}, ItemDropHandler::GetRandomScaledItem(15)));
    }

    PLAYER_HOTBAR[0]->skill = SKILLS[VOID_FIELD];
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
std::array<std::function<void()>, 6> GameLoader::load_functions = {
    EntityLoader::Load,  GuiLoadStyleAshes, TileLoader::LoadToGPU,
    TextureLoader::Load, ItemLoader::Load,  TalentLoader::Load};

#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
