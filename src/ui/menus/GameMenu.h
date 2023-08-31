#ifndef DND_SRC_UI_GAMEMENU_H_
#define DND_SRC_UI_GAMEMENU_H_

#include "../Component.h"
#include "SettingsMenu.h"

struct GameMenu : public Component {
  MenuState menu_state = MenuState::Main;
  GameState* game_state;
  SettingsMenu settings_menu;

  GameMenu(int* screen_width, int* screen_height, float* ui_scale, GameState* game_state,
           int* targetFPS, bool* showFPS)
      : settings_menu(screen_width, screen_height, ui_scale, targetFPS, showFPS),
        game_state(game_state) {}
  void draw(float ui_scale, int screen_width, int screen_height) final {
    DrawRectangle(0, 0, screen_width, screen_height, ColorAlpha(GRAY, 0.7));
    const float scaled_width = 110 * ui_scale;
    const float scaled_height = 30 * ui_scale;
    const float vertical_gap = 10 * ui_scale;
    const float xOffset = (screen_width / 2.0f) - (scaled_width / 2.0f);

    if (menu_state == MenuState::Main) {
      if (GuiButton({xOffset, (screen_height / 2.0f) - (3 * scaled_height) - (2 * vertical_gap),
                     scaled_width, scaled_height},
                    "Resume")) {
        *game_state = GameState::Game;
        menu_state = MenuState::Main;
      }
      if (GuiButton({xOffset, (screen_height / 2.0f) - (2 * scaled_height) - vertical_gap,
                     scaled_width, scaled_height},
                    "Settings")) {
        menu_state = MenuState::Settings;
      }
      if (GuiButton({xOffset, (screen_height / 2.0f) - scaled_height, scaled_width, scaled_height},
                    "Back to MainMenu")) {
        *game_state = GameState::MainMenu;
        menu_state = MenuState::Main;
      }
    } else if (menu_state == MenuState::Settings) {
      settings_menu.draw(ui_scale, screen_width, screen_height);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      menu_state = MenuState::Main;
    }
  }
};
#endif  //DND_SRC_UI_GAMEMENU_H_
