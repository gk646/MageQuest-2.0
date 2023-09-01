#ifndef DND_SRC_UI_MAINMENU_H_
#define DND_SRC_UI_MAINMENU_H_

#include <thread>
#include "../../util/Enums.h"
#include "ServerBrowser.h"
#include "SettingsMenu.h"

struct MainMenu {
  ServerBrowser server_browser{};
  SettingsMenu& settings_menu;
  MenuState menu_state = MenuState::Main;

  MainMenu(SettingsMenu& settings_menu) : settings_menu(settings_menu) {}
  void draw() noexcept {
    const float scaled_width = 110 * UI_SCALE;
    const float scaled_height = 30 * UI_SCALE;
    const float vertical_gap = 10 * UI_SCALE;
    const float xOffset = SCREEN_WIDTH / 2.0 - (scaled_width / 2.0);
    const float baseYOffset =
        static_cast<float>(SCREEN_HEIGHT / 2.0) - (3 * scaled_height) - (4 * vertical_gap);

    if (menu_state == MenuState::Main) {
      if (GuiButton({xOffset, baseYOffset, scaled_width, scaled_height}, "Play Game")) {
        GAME_STATE = GameState::Game;
      }
      if (GuiButton(
              {xOffset, baseYOffset + (scaled_height + vertical_gap), scaled_width, scaled_height},
              "Host Game")) {}
      if (GuiButton({xOffset, baseYOffset + 2 * (scaled_height + vertical_gap), scaled_width,
                     scaled_height},
                    "Join Game")) {
        menu_state = MenuState::ServerBrowser;
        std::thread myThread(&ServerBrowser::scanNetwork, &server_browser);
        myThread.detach();
      }
      if (GuiButton({xOffset, baseYOffset + 3 * (scaled_height + vertical_gap), scaled_width,
                     scaled_height},
                    "Settings")) {
        menu_state = MenuState::Settings;
      }
      if (GuiButton({xOffset, baseYOffset + 4 * (scaled_height + vertical_gap), scaled_width,
                     scaled_height},
                    "Exit")) {
        CloseWindow();
        exit(EXIT_SUCCESS);
      }
    } else if (menu_state == MenuState::Settings) {
      settings_menu.draw();
    } else if (menu_state == MenuState::ServerBrowser) {
      server_browser.draw();
    } else if (menu_state == MenuState::HostGame) {
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      menu_state = MenuState::Main;
    }
  }
};
#endif  //DND_SRC_UI_MAINMENU_H_
