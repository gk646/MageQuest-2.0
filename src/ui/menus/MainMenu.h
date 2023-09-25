#ifndef MAGE_QUEST_SRC_UI_MAINMENU_H_
#define MAGE_QUEST_SRC_UI_MAINMENU_H_

struct MainMenu {
  ServerBrowser server_browser{};
  HostMenu host_menu{};
  SettingsMenu& settings_menu;
  MenuState menu_state = MenuState::Main;
  Button playGame{144, 46, "Play Game", 20};
  explicit MainMenu(SettingsMenu& settings_menu) noexcept
      : settings_menu(settings_menu) {}
  void draw() noexcept {
    const float scaled_width = 110 * UI_SCALE;
    const float scaled_height = 30 * UI_SCALE;
    const float vertical_gap = 10 * UI_SCALE;
    const float xOffset = SCREEN_WIDTH / 2.0F - (scaled_width / 2.0);
    const float baseYOffset = SCREEN_HEIGHT / 2.0F - 3 * scaled_height - 4 * vertical_gap;
    DrawRectanglePro(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, 0, Colors::LightGrey);
    if (!IsSoundPlaying(sound::intro)) {
      PlaySoundR(sound::intro);
    }
    if (menu_state == MenuState::Main) {
      DrawCenteredText(VARNISHED, 35, "Mage Quest II", SCREEN_WIDTH / 2, 150, Colors::purple_dark);
      if (playGame.Draw(SCREEN_WIDTH / 2.0F, baseYOffset)) {
        GAME_STATE = GameState::Game;
        StopSound(sound::intro);
      }
      if (GuiButton({xOffset, baseYOffset + (scaled_height + vertical_gap), scaled_width,
                     scaled_height},
                    "Host Game")) {
        menu_state = MenuState::HostGame;
        SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, 3);
      }
      if (GuiButton({xOffset, baseYOffset + 2 * (scaled_height + vertical_gap),
                     scaled_width, scaled_height},
                    "Join Game")) {
        menu_state = MenuState::ServerBrowser;
      }
      if (GuiButton({xOffset, baseYOffset + 3 * (scaled_height + vertical_gap),
                     scaled_width, scaled_height},
                    "Settings")) {
        menu_state = MenuState::Settings;
      }
      if (GuiButton({xOffset, baseYOffset + 4 * (scaled_height + vertical_gap),
                     scaled_width, scaled_height},
                    "Exit")) {
        SetWindowCloseFlagTrue(1);
      }
    } else if (menu_state == MenuState::Settings) {
      settings_menu.draw();
    } else if (menu_state == MenuState::ServerBrowser) {
      server_browser.draw();
    } else if (menu_state == MenuState::HostGame) {
      host_menu.DrawHostMenu();
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      menu_state = MenuState::Main;
      Multiplayer::CloseMultiplayer();
    }
  }
};
#endif  //MAGE_QUEST_SRC_UI_MAINMENU_H_
