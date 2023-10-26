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
#include "loading/loaders/SkillLoader.h"

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
    LoadStep(SkillLoader::Load);
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
    PLAYER_STATS.RefillStats();
#ifdef SPAWN_TESTROOM
    WorldManager::LoadMap(Zone::TestRoom, {24, 34});
#else
    WorldManager::LoadMap(Zone::Oasis, {4, 96});
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
    UI_MANAGER.playerUI.playerHotbar.skills[1] = SKILLS[FIRE_STRIKE];
    UI_MANAGER.playerUI.playerHotbar.skills[2] = SKILLS[BLAST_HAMMER];
    UI_MANAGER.playerUI.playerHotbar.skills[4] = SKILLS[ENERGY_SPHERE];
    UI_MANAGER.playerUI.playerHotbar.skills[5] = SKILLS[FROST_NOVA];
    NPCS.push_back(NPC::GetNewNPC(NPC_ID::SATIRO, 4 * 48, 96 * 48, CURRENT_ZONE));
    for (uint_fast32_t i = 0; i < 20; i++) {
      // MONSTERS.push_back(          Monster::GetNewMonster(250.0F + i * 5, 150, MonsterType::SKEL_ARCHER, 5));
      MONSTERS.push_back(
          Monster::GetNewMonster(250.0F + i * 5, 150, MonsterType::GHOST, 30));
      WORLD_OBJECTS.push_back(new DroppedItem(
          {4 * 48, 96 * 48},
          ItemDropHandler::CreateNewScaledItem(
              ItemDropHandler::GetRandomItemPtr(ItemType::BAG), 100, 30)));
      ItemDropHandler::RollForItemDrop(4 * 48, 96 * 48, 30);
    }
  }
};
std::atomic_bool GameLoader::finished_cpu_loading{false};
std::array<std::function<void()>, 5> GameLoader::load_functions = {
    EntityLoader::Load, GuiLoadStyleAshes, TileLoader::load_to_vram, TextureLoader::load,
    DataBaseHandler::load};

#endif  //MAGE_QUEST_SRC_LOADING_STARTUPLOADER_H_
