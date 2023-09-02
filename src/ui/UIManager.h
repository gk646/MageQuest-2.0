#ifndef MAGE_QUEST_SRC_UI_UIMANAGER_H_
#define MAGE_QUEST_SRC_UI_UIMANAGER_H_

#include "game/LoadingScreen.h"

#include "menus/GameMenu.h"
#include "menus/MainMenu.h"

#include "menus/SettingsMenu.h"
#include "player/PlayerUI.h"

struct UIManager {
  LoadingScreen loading_screen;
  PlayerUI player_ui;
  SettingsMenu settings_menu;
  GameMenu game_menu{settings_menu};
  MainMenu main_menu{settings_menu};

  void draw() noexcept {
    ui_update();
    if (GAME_STATE == GameState::Game) {
      player_ui.draw();
    } else if (GAME_STATE == GameState::GameMenu) {
      player_ui.draw();
      game_menu.draw();
    } else if (GAME_STATE == GameState::MainMenu) {
      main_menu.draw();
    } else if (GAME_STATE == GameState::Loading) {
      if (GameLoader::finished_cpu_loading) {
        GameLoader::finish_loading();
      }
      loading_screen.draw();
    }
    if (settings_menu.showFPS) {
      DrawFPS(25, 25);
    }
  }
  void ui_update() const {
    if (!IsWindowFullscreen()) {
      if (GetScreenWidth() != SCREEN_WIDTH) {
        SCREEN_WIDTH = GetScreenWidth();
        CAMERA_X = SCREEN_WIDTH / 2;
      }
      if (GetScreenHeight() != SCREEN_HEIGHT) {
        SCREEN_HEIGHT = GetScreenHeight();
        CAMERA_Y = SCREEN_HEIGHT / 2;
      }
    }
  }

  void update() {
    if (IsKeyPressed(KEY_ESCAPE)) {
      if (GAME_STATE == GameState::GameMenu && game_menu.menu_state == MenuState::Main) {
        GAME_STATE = GameState::Game;
      } else if (GAME_STATE == GameState::Game) {
        GAME_STATE = GameState::GameMenu;
      }
    }
    if (GAME_STATE == GameState::Game) {
      player_ui.update();
    }
  }
};
#endif  //MAGE_QUEST_SRC_UI_UIMANAGER_H_
