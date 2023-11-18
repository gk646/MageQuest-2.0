#ifndef MAGE_QUEST_SRC_UI_UIMANAGER_H_
#define MAGE_QUEST_SRC_UI_UIMANAGER_H_

namespace UIFunctions {
inline void RestUIPosition();
}

#include "elements/SlideComponent.h"
#include "elements/Content.h"
#include "elements/ScrollPane.h"
#include "elements/SoundComponent.h"
#include "elements/TextField.h"
#include "elements/TextPanel.h"

#include "PlayerUI.h"
#include "menus/SettingsMenu.h"
#include "menus/MainMenu.h"
#include "menus/GameMenu.h"

struct UIManager {
  PlayerUI playerUI;
  SettingsMenu settings_menu;
  GameMenu gameMenu{settings_menu, playerUI};
  MainMenu mainMenu{settings_menu};
  //Assigns functions to menu buttons
  UIManager() {
    playerUI.playerHotbar.menuButtons[0].onPressedFunc = [&]() {
      playerUI.charPanel.ToggleWindow();
    };
    playerUI.playerHotbar.menuButtons[1].onPressedFunc = [&]() {
      playerUI.charBag.ToggleWindow();
    };
    playerUI.playerHotbar.menuButtons[2].onPressedFunc = [&]() {
      playerUI.questPanel.ToggleWindow();
    };
    playerUI.playerHotbar.menuButtons[3].onPressedFunc = [&]() {
      playerUI.skillPanel.ToggleWindow();
    };
    playerUI.playerHotbar.menuButtons[4].onPressedFunc = [&]() {
      playerUI.talentPanel.ToggleWindow();
    };
    playerUI.playerHotbar.menuButtons[5].onPressedFunc = [&]() {
      playerUI.regionMap.ToggleWindow();
    };
    playerUI.playerHotbar.menuButtons[6].onPressedFunc = [&]() {
      GAME_STATE = GameState::GameMenu;
    };
  }
  inline void UIUpdate() noexcept {
    MOUSE_POS = GetMousePosition();
    if (!IsWindowFullscreen()) {
      if (GetScreenWidth() != SCREEN_WIDTH || GetScreenHeight() != SCREEN_HEIGHT) {
        SCREEN_WIDTH = GetScreenWidth();
        SCREEN_HEIGHT = GetScreenHeight();
        UnloadRenderTexture(FIRST_LAYER_BUFFER);
        FIRST_LAYER_BUFFER = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
      }
    }
    if (IsKeyPressed(KEY_ESCAPE) && GAME_STATE == GameState::Game) {
      gameMenu.Open();
    }
  }
  inline void Update() noexcept { playerUI.Update(); }
  inline void ResetUIPosition() { playerUI.ResetPosition(); }
};

inline UIManager UI_MANAGER{};

namespace UIFunctions {
inline void RestUIPosition() {
  UI_SCALE = 1;
  UI_MANAGER.ResetUIPosition();
}
}  // namespace UIFunctions

#endif  //MAGE_QUEST_SRC_UI_UIMANAGER_H_
