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
#include "ui/StyleSheet.h"

#include <thread>
#include <vector>

using namespace std::chrono_literals;

class Game {
  int targetFPS = 120;
  bool logic_thread_running = true;
  bool multiplayer = false;

  std::vector<Player> other_players;
  std::vector<Projectile> projectiles;
  std::vector<Player> npcs;
  std::vector<Monster> monsters;

  std::thread logic_thread;

  UIManager ui_manager{};

  Player player;

  void render() noexcept {
    while (!(WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))) {
      BeginDrawing();
      ClearBackground(WHITE);
      if (GAME_STATE == GameState::Game || GAME_STATE == GameState::GameMenu) {
        drawGame();
      }
      ui_manager.draw();
      EndDrawing();
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
    erase_if(projectiles, [&](const auto& item) { return item.dead; });
    erase_if(monsters, [&](const auto& monster) { return monster.dead; });

    if (GAME_STATE == GameState::Game) {
      player.update(&projectiles);
    }

    PLAYER_TILE_X = (player.pos.x() + player.size.x() / 2) / TILE_SIZE;
    PLAYER_TILE_Y = (player.pos.y() + player.size.y() / 2) / TILE_SIZE;

    for (auto& monster : monsters) {
      monster.update();
    }

    for (auto& projectile : projectiles) {
      projectile.update();
      for (auto& monster : monsters) {
        if (!projectile.dead && !monster.dead && projectile.intersects(monster)) {
          monster.hit(projectile.damage_stats, &projectile.dead);
        }
      }
    }
  }
  inline void drawGame() noexcept {
    CAMERA_X = SCREEN_WIDTH / 2;
    CAMERA_Y = SCREEN_HEIGHT / 2;
    WorldRender::draw();

    for (auto& projectile : projectiles) {
      projectile.draw();
    }
    for (auto& npc : npcs) {
      npc.draw();
    }
    for (auto& monster : monsters) {
      monster.draw();
    }
    for (auto players : other_players) {
      players.draw();
    }
    player.draw();
    WorldRender::draw_fore_ground();
  }

  void loadResources() {
    Image icon = LoadImage((ASSET_PATH + "dnd_logo.png").c_str());
    SetWindowIcon(icon);
    UnloadImage(icon);

    GameLoader::load();
  }

 public:
  Game() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(targetFPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mage Quest 2");

    loadResources();
    player = Player(Class::DRUID, {500, 500}, {25, 25}, ShapeType::CIRCLE);
    other_players.push_back(Player(Class::DRUID, {500, 500}));
    monsters.push_back(
        Monster({200, 200}, EntityStats({}, {1, 2, 100, 4}), {50, 50}, ShapeType::RECT));
    GuiLoadStyleCyber();
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
    logic_thread = std::thread(&Game::game_logic, this);
    render();
  }
};
#endif  //MAGE_QUEST_SRC_GAME_H_
