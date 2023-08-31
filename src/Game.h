#ifndef DND_SRC_GAME_H_
#define DND_SRC_GAME_H_

#define RAYLIB_IMPLEMENTATION
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "cxstructs/Geometry.h"
#include "cxstructs/vec.h"

#include "enums/Enums.h"
#include "gameobjects/entities/types/Monster.h"
#include "gameobjects/entities/types/Player.h"
#include "gameobjects/entities/types/Projectile.h"
#include "graphics/BattleMap.h"
#include "ui/Component.h"
#include "ui/menus/GameMenu.h"
#include "ui/menus/MainMenu.h"
#include "util/MathUtil.h"

#include "util/GlobalVariables.h"

#include <thread>
#include <vector>

using namespace std::chrono_literals;

class Game {

  bool drawFPS = false;
  int targetFPS = 120;
  bool logic_thread_running = true;
  bool multiplayer = false;

  GameState game_state = GameState::MainMenu;
  Texture2D main_menu_background;

  Player player;

  std::vector<Player> other_players;
  std::vector<Projectile> projectiles;
  std::vector<Player> npcs;
  std::vector<Monster> monsters;

  std::thread logic_thread;

  Point camera_pos = {0, 0};

  GameMenu* game_menu =
      new GameMenu(&SCREEN_WIDTH, &SCREEN_HEIGHT, &UI_SCALE, &game_state, &targetFPS, &drawFPS);
  MainMenu* main_menu =
      new MainMenu(&SCREEN_WIDTH, &SCREEN_HEIGHT, &UI_SCALE, &game_state, &targetFPS, &drawFPS);

  void render() {
    while (!(WindowShouldClose() && !IsKeyDown(KEY_ESCAPE))) {
      BeginDrawing();
      ClearBackground(WHITE);
      if (game_state == GameState::Game || game_state == GameState::GameMenu) {
        drawGame();
      }
      drawMenus();
      EndDrawing();
    }
    CloseWindow();
  }
  void game_logic() {
    while (logic_thread_running) {
      PollInputEvents();

      if(game_state == GameState::Game){
        player.update(&projectiles);
      }

      for (auto& projectile : projectiles) {
        projectile.update();
        for (auto& monster : monsters) {
          if (!monster.dead && projectile.intersects(monster)) {
            monster.hit(projectile.damage_stats);
          }
        }
      }
      for (auto& monster : monsters) {
        monster.update();
      }
      erase_if(projectiles, [&](const auto& item) { return item.dead; });
      erase_if(monsters, [&](const auto& monster) { return monster.dead; });
      std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
  }
  void drawMenus() {
    if (!IsWindowFullscreen()) {
      if (GetScreenWidth() != SCREEN_WIDTH) {
        SCREEN_WIDTH = GetScreenWidth();
      }
      if (GetScreenHeight() != SCREEN_HEIGHT) {
        SCREEN_HEIGHT = GetScreenHeight();
      }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      if (game_state == GameState::GameMenu && game_menu->menu_state == MenuState::Main) {
        game_state = GameState::Game;
      } else if (game_state == GameState::Game) {
        game_state = GameState::GameMenu;
      }
    }
    if (game_state == GameState::MainMenu) {
      DrawTexture(main_menu_background, 0, 0, WHITE);
      main_menu->draw(UI_SCALE, SCREEN_WIDTH, SCREEN_HEIGHT);
    } else if (game_state == GameState::Game || game_state == GameState::GameMenu) {

      if (game_state == GameState::GameMenu) {
        game_menu->draw(UI_SCALE, SCREEN_WIDTH, SCREEN_HEIGHT);
      }
    }
    if (drawFPS) {
      DrawFPS(25, 25);
    }
  }
  void drawGame() {
    //map.draw(SCREEN_WIDTH, SCREEN_HEIGHT, camera_pos);
    for (auto& projectiles : projectiles) {
      projectiles.draw();
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
  }

  void loadResources() {
    Image image = LoadImage("res/main_menu.png");
    ImageResize(&image, GetMonitorWidth(0), GetMonitorHeight(0));
    main_menu_background = LoadTextureFromImage(image);
    UnloadImage(image);

    Image icon = LoadImage("res/dnd_logo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
  }

 public:
  Game() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(targetFPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DnD - 2D Adventure");

    loadResources();
    player = Player(Class::DRUID, {500, 500});
    other_players.push_back(Player(Class::DRUID, {500, 500}));
    monsters.push_back(Monster({200, 200}, EntityStats({}, {1, 2, 1000, 4})));
    //other_players.push_back(Player(Class::BARBARIAN, {200, 200}, {50, 50}, 0, "Jana"));
  }
  ~Game() {
    UnloadTexture(main_menu_background);
    logic_thread_running = false;
    logic_thread.join();
  }
  void start() {
    logic_thread = std::thread(&Game::game_logic, this);
    render();
  }
};
#endif  //DND_SRC_GAME_H_
