#ifndef MAGE_QUEST_SRC_UI_UIMANAGER_H_
#define MAGE_QUEST_SRC_UI_UIMANAGER_H_

#include "elements/Button.h"
#include "elements/Panel.h"
namespace UIFunction{
  inline void RestUIPosition();
}

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
    MOUSE_POS = GetMousePosition();
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
  inline void update() noexcept { player_ui.update(); }
  inline void ResetUIPosition(){
    player_ui.ResetPosition();
  }
};

inline UIManager UI_MANAGER{};

namespace UIFunction{
inline void RestUIPosition(){
  UI_MANAGER.ResetUIPosition();
}
}

#endif  //MAGE_QUEST_SRC_UI_UIMANAGER_H_
