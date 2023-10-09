#ifndef MAGE_QUEST_SRC_UI_UIMANAGER_H_
#define MAGE_QUEST_SRC_UI_UIMANAGER_H_

namespace UIFunctions {
inline void RestUIPosition();
}

#include "elements/TextCell.h"
#include "elements/Panel.h"

#include "PlayerUI.h"
#include "menus/SettingsMenu.h"
#include "menus/MainMenu.h"
#include "menus/GameMenu.h"

struct UIManager {
  PlayerUI player_ui;
  SettingsMenu settings_menu;
  GameMenu game_menu{settings_menu};
  MainMenu main_menu{settings_menu};
  UIManager() {
    player_ui.playerHotbar.menuButtons[0].onPressedFunc = [&]() {
      player_ui.char_bag.ToggleWindow();
    };
    player_ui.playerHotbar.menuButtons[1].onPressedFunc = [&]() {
      player_ui.char_bag.ToggleWindow();
    };
    player_ui.playerHotbar.menuButtons[2].onPressedFunc = [&]() {
      player_ui.char_panel.ToggleWindow();
    };
    player_ui.playerHotbar.menuButtons[3].onPressedFunc = [&]() {
      player_ui.region_map.ToggleWindow();
    };
    player_ui.playerHotbar.menuButtons[4].onPressedFunc = [&]() {
      GAME_STATE = GameState::GameMenu;
    };
    player_ui.playerHotbar.menuButtons[5].onPressedFunc = [&]() {
      player_ui.char_bag.ToggleWindow();
    };
  }
  void UIUpdate() noexcept {
    MOUSE_POS = GetMousePosition();
    if (!IsWindowFullscreen()) {
      if (GetScreenWidth() != SCREEN_WIDTH || GetScreenHeight() != SCREEN_HEIGHT) {
        SCREEN_WIDTH = GetScreenWidth();
        SCREEN_HEIGHT = GetScreenHeight();
        UnloadRenderTexture(FIRST_LAYER_BUFFER);
        FIRST_LAYER_BUFFER = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
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
  inline void Update() noexcept { player_ui.Update(); }
  inline void ResetUIPosition() { player_ui.ResetPosition(); }
};

inline UIManager UI_MANAGER{};

namespace UIFunctions {
inline void RestUIPosition() {
  UI_SCALE = 1;
  UI_MANAGER.ResetUIPosition();
}
}  // namespace UIFunctions

#endif  //MAGE_QUEST_SRC_UI_UIMANAGER_H_
