#ifndef MAGE_QUEST_SRC_GAME_H_
#define MAGE_QUEST_SRC_GAME_H_

#define RAYLIB_IMPLEMENTATION
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "system/GlobalVariables.h"

#include "cxstructs/Geometry.h"
#include "cxstructs/vec.h"
#include "cxutil/cxtime.h."

#include "gameobjects/entities/types/Monster.h"
#include "gameobjects/entities/types/Player.h"
#include "gameobjects/entities/types/Projectile.h"
#include "gamestateio/GameLoader.h"
#include "ui/UIManager.h"
#include "util/Enums.h"
#include "util/Util.h"

#include "graphics/WorldRender.h"

#include <thread>
#include <vector>

using namespace std::chrono_literals;

class Game {
  int targetFPS = 120;
  bool logic_thread_running = true;
  bool multiplayer = false;

  std::thread logic_thread;

  Player player;

  UIManager ui_manager{};

  void render() noexcept {
    while (!(WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))) {
      cxstructs::now(0);
      BeginDrawing();
      ClearBackground(WHITE);
      if (GAME_STATE == GameState::Game || GAME_STATE == GameState::GameMenu) {
        drawGame();
      }
      ui_manager.draw();
      EndDrawing();
      FRAME_TIME = cxstructs::getTime(0);
    }
  }
  void game_logic() noexcept {
    auto nextUpdate = std::chrono::high_resolution_clock::now();
    auto targetDuration = std::chrono::milliseconds(16);

    while (logic_thread_running) {
      auto currentTime = std::chrono::high_resolution_clock::now();

      if (currentTime >= nextUpdate) {
        game_tick();
        nextUpdate = nextUpdate + targetDuration;

        while (nextUpdate < std::chrono::high_resolution_clock::now()) {
          nextUpdate = nextUpdate + targetDuration;
        }
      } else {
        std::this_thread::sleep_until(nextUpdate);
      }
    }
  }

  inline void game_tick() noexcept {
    cxstructs::now();
    erase_if(PROJECTILES, [&](const auto& item) { return item.dead; });
    erase_if(MONSTERS, [&](const auto& monster) { return monster.dead; });

    if (GAME_STATE == GameState::Game) {
      player.update();
    }
    ui_manager.update();

    for (auto& monster : MONSTERS) {
      monster.update();
    }

    for (auto& projectile : PROJECTILES) {
      projectile.update();
      for (auto& monster : MONSTERS) {
        if (!projectile.dead && !monster.dead && projectile.intersects(monster)) {
           monster.hit(projectile);
        }
      }
      if (projectile.intersects(player)) {
        player.hit(projectile);
      }
    }
    GAME_TICK_TIME = cxstructs::getTime<std::chrono::nanoseconds>();
  }
  inline void drawGame() noexcept {
    CAMERA_X = SCREEN_WIDTH / 2;
    CAMERA_Y = SCREEN_HEIGHT / 2;
    WorldRender::draw();

    for (auto& projectile : PROJECTILES) {
      projectile.draw();
    }
    for (auto& npc : NPCS) {
      npc.draw();
    }
    for (auto& monster : MONSTERS) {
      monster.draw();
    }
    for (auto players : OTHER_PLAYERS) {
      players.draw();
    }
    player.draw();
    WorldRender::draw_fore_ground();
  }

 public:
  Game() : player(Class::DRUID, {200, 200}, {25, 25}, ShapeType::CIRCLE) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(targetFPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mage Quest 2");

    //SettingsMenu::set_full_screen();
  }
  ~Game() {
    for (uint_fast32_t i = 0; i < 5589; i++) {
      UnloadTexture(TEXTURES[i]);
    }
    logic_thread_running = false;
    logic_thread.join();
    CloseWindow();
  }
  void start() {
    GameLoader::load();
    logic_thread = std::thread(&Game::game_logic, this);
    render();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
