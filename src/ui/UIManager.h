#ifndef MAGE_QUEST_SRC_UI_UIMANAGER_H_
#define MAGE_QUEST_SRC_UI_UIMANAGER_H_

#include "PlayerUI.h"

struct UIManager {
  PlayerUI player_ui;
  SettingsMenu settings_menu;
  GameMenu game_menu{settings_menu};
  MainMenu main_menu{settings_menu};

  void ui_update() noexcept {
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

    if (IsKeyPressed(KEY_ESCAPE)) {
      if (GAME_STATE == GameState::GameMenu &&
          game_menu.menu_state == MenuState::Main) {
        GAME_STATE = GameState::Game;
      } else if (GAME_STATE == GameState::Game) {
        if (!player_ui.window_closeable()) {
          GAME_STATE = GameState::GameMenu;
        }
      }
    }
  }

  inline void update() noexcept { player_ui.update(); }
};

inline UIManager UI_MANAGER{};

#endif  //MAGE_QUEST_SRC_UI_UIMANAGER_H_
