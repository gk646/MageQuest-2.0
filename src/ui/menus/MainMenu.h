#ifndef MAGE_QUEST_SRC_UI_MAINMENU_H_
#define MAGE_QUEST_SRC_UI_MAINMENU_H_

struct MainMenu {
  ServerBrowser server_browser{};
  HostMenu host_menu{};
  SettingsMenu& settings_menu;
  MenuState menu_state = MenuState::Main;
  Button playGame{144, 46, "Play Game", 20, textures::ui::buttonNormal,textures::ui::buttonHovered, textures::ui::buttonPressed};
  Button hostGame{144, 46, "Host Game", 20, textures::ui::buttonNormal,textures::ui::buttonHovered, textures::ui::buttonPressed};
  Button joinGame{144, 46, "Join Game", 20, textures::ui::buttonNormal,textures::ui::buttonHovered, textures::ui::buttonPressed};
  Button settings{144, 46, "Settings", 20,  textures::ui::buttonNormal,textures::ui::buttonHovered, textures::ui::buttonPressed};
  Button exit{144, 46, "Quit", 20,  textures::ui::buttonNormal,textures::ui::buttonHovered, textures::ui::buttonPressed};
  explicit MainMenu(SettingsMenu& settings_menu) noexcept
      : settings_menu(settings_menu) {}
  void draw() noexcept {
    const float scaled_height = SCALE(46);
    const float vertical_gap = SCALE(8);
    const float baseYOffset = SCREEN_HEIGHT / 2.0F - 2 * scaled_height - 4 * vertical_gap;
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
      if (hostGame.Draw(SCREEN_WIDTH / 2.0F, baseYOffset + (scaled_height + vertical_gap))) {
        menu_state = MenuState::HostGame;
        SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, 3);
      }
      if (joinGame.Draw(SCREEN_WIDTH / 2.0F, baseYOffset + 2 * (scaled_height + vertical_gap))) {
        menu_state = MenuState::ServerBrowser;
      }
      if (settings.Draw(SCREEN_WIDTH / 2.0F, baseYOffset + 3 * (scaled_height + vertical_gap))) {
        menu_state = MenuState::Settings;
      }
      if (exit.Draw(SCREEN_WIDTH / 2.0F, baseYOffset + 4 * (scaled_height + vertical_gap))) {
        SetWindowCloseFlagTrue(1);
      }
    } else if (menu_state == MenuState::Settings) {
      settings_menu.Draw();
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
