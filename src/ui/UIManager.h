#ifndef MAGE_QUEST_SRC_UI_UIMANAGER_H_
#define MAGE_QUEST_SRC_UI_UIMANAGER_H_

#include "elements/Button.h"
#include "PlayerUI.h"
#include "menus/SettingsMenu.h"
#include "menus/MainMenu.h"
#include "menus/GameMenu.h"

struct UIManager {
  PlayerUI player_ui;
  SettingsMenu settings_menu;
  GameMenu game_menu{settings_menu};
  MainMenu main_menu{settings_menu};

  void ui_update() noexcept {
    if (!IsWindowFullscreen()) {
      if (GetScreenWidth() != SCREEN_WIDTH) {
        SCREEN_WIDTH = GetScreenWidth();
      }
      if (GetScreenHeight() != SCREEN_HEIGHT) {
        SCREEN_HEIGHT = GetScreenHeight();
      }
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      if (GAME_STATE == GameState::GameMenu && game_menu.menu_state == MenuState::Main) {
        GAME_STATE = GameState::Game;
      } else if (GAME_STATE == GameState::Game) {
        if (!player_ui.window_closeable()) {
          GAME_STATE = GameState::GameMenu;
        }
      }
    }
  }

  inline void update() noexcept {
    MOUSE_POS = GetMousePosition();
    player_ui.update();
  }
};

inline UIManager UI_MANAGER{};

#endif  //MAGE_QUEST_SRC_UI_UIMANAGER_H_
