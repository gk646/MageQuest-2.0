#ifndef MAGE_QUEST_SRC_UI_GAMEMENU_H_
#define MAGE_QUEST_SRC_UI_GAMEMENU_H_

struct GameMenu {
  MenuState menu_state = MenuState::Main;
  SettingsMenu& settings_menu;

  explicit GameMenu(SettingsMenu& settings_menu) noexcept
      : settings_menu(settings_menu) {}
  void draw() noexcept {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(GRAY, 0.7));
    const float scaled_width = 110 * UI_SCALE;
    const float scaled_height = 30 * UI_SCALE;
    const float vertical_gap = 10 * UI_SCALE;
    const float xOffset = (SCREEN_WIDTH / 2.0f) - (scaled_width / 2.0f);

    if (menu_state == MenuState::Main) {
      if (GuiButton(
              {xOffset, (SCREEN_HEIGHT / 2.0f) - (4 * scaled_height) - (3 * vertical_gap),
               scaled_width, scaled_height},
              "Resume")) {
        GAME_STATE = GameState::Game;
        menu_state = MenuState::Main;
      }
      if (GuiButton(
              {xOffset, (SCREEN_HEIGHT / 2.0f) - (3 * scaled_height) - 2 * vertical_gap,
               scaled_width, scaled_height},
              "Settings")) {
        menu_state = MenuState::Settings;
      }
      if (GuiButton({xOffset, (SCREEN_HEIGHT / 2.0f) - (2 * scaled_height) - vertical_gap,
                     scaled_width, scaled_height},
                    "Reset UI")) {
        UIFunctions::RestUIPosition();
      }
      if (GuiButton({xOffset, (SCREEN_HEIGHT / 2.0f) - scaled_height, scaled_width,
                     scaled_height},
                    "Back to MainMenu")) {
        GAME_STATE = GameState::MainMenu;
        menu_state = MenuState::Main;
        PlaySoundR(sound::intro);
        Multiplayer::CloseMultiplayer();
      }
    } else if (menu_state == MenuState::Settings) {
      settings_menu.Draw();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      PlaySoundR(sound::menu_back);
      menu_state = MenuState::Main;
    }
  }
};
#endif  //MAGE_QUEST_SRC_UI_GAMEMENU_H_
