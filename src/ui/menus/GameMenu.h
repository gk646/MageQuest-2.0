#ifndef MAGE_QUEST_SRC_UI_GAMEMENU_H_
#define MAGE_QUEST_SRC_UI_GAMEMENU_H_

struct GameMenu {
  MenuState menuState = MenuState::Main;
  SettingsMenu& settingsMenu;
  PlayerUI& playerUI;

#define Close()                 \
  PlaySoundR(sound::menu_back); \
  GAME_STATE = GameState::Game; \
  menuState = MenuState::Main;  \
  return;

  explicit GameMenu(SettingsMenu& settings_menu, PlayerUI& playerUI) noexcept
      : settingsMenu(settings_menu), playerUI(playerUI) {}
  void Draw() noexcept {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ColorAlpha(GRAY, 0.7));
    const float scaled_width = 110 * UI_SCALE;
    const float scaled_height = 30 * UI_SCALE;
    const float vertical_gap = 10 * UI_SCALE;
    const float xOffset = (SCREEN_WIDTH / 2.0f) - (scaled_width / 2.0f);

    if (menuState == MenuState::Main) {
      if (GuiButton(
              {xOffset, (SCREEN_HEIGHT / 2.0f) - (4 * scaled_height) - (3 * vertical_gap),
               scaled_width, scaled_height},
              "Resume")) {
        Close();
      }
      if (GuiButton(
              {xOffset, (SCREEN_HEIGHT / 2.0f) - (3 * scaled_height) - 2 * vertical_gap,
               scaled_width, scaled_height},
              "Settings")) {
        menuState = MenuState::Settings;
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
        menuState = MenuState::Main;
        PlaySoundR(sound::intro);
        Multiplayer::CloseMultiplayer();
      }
    } else if (menuState == MenuState::Settings) {
      settingsMenu.Draw();
    }

    if (IsKeyPressed(KEY_ESCAPE) && menuState != MenuState::Transition) {
      if (menuState == MenuState::Main) {
        Close()
      }
      PlaySoundR(sound::menu_back);
      menuState = MenuState::Main;
    }
    if (menuState == MenuState::Transition) {
      menuState = MenuState::Main;
    }
  }
  inline void Open() noexcept {
    if (playerUI.window_closeable()) return;
    PlaySoundR(sound::menu_switch);
    GAME_STATE = GameState::GameMenu;
    menuState = MenuState::Transition;
  }
};
#endif  //MAGE_QUEST_SRC_UI_GAMEMENU_H_
