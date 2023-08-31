#ifndef DND_SRC_UI_MAINMENU_H_
#define DND_SRC_UI_MAINMENU_H_

#include <thread>
#include "../../enums/Enums.h"
#include "../Component.h"
#include "ServerBrowser.h"
#include "SettingsMenu.h"

struct MainMenu : public Component {

  ServerBrowser server_browser{};
  SettingsMenu settings_menu;
  MenuState menu_state = MenuState::Main;
  GameState* game_state;

  MainMenu(int* screen_width, int* screen_height, float* ui_scale, GameState* game_state,
           int* targetFPS, bool* showFPS)
      : settings_menu(screen_width, screen_height, ui_scale, targetFPS, showFPS),
        game_state(game_state) {}
  void draw(float ui_scale, int screen_width, int screen_height) final {
    const float scaled_width = 110 * ui_scale;
    const float scaled_height = 30 * ui_scale;
    const float vertical_gap = 10 * ui_scale;  // Adding vertical gap using ui_scale
    const float xOffset = static_cast<float>(screen_width / 2.0) - (scaled_width / 2.0);
    const float baseYOffset =
        static_cast<float>(screen_height / 2.0) - (3 * scaled_height) - (4 * vertical_gap);

    if (menu_state == MenuState::Main) {
      if (GuiButton({xOffset, baseYOffset, scaled_width, scaled_height}, "Play Game")) {
        *game_state = GameState::Game;
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
      settings_menu.draw(ui_scale, screen_width, screen_height);
    } else if (menu_state == MenuState::ServerBrowser) {
      server_browser.draw(ui_scale, screen_width, screen_height);
    } else if (menu_state == MenuState::HostGame) {
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
      menu_state = MenuState::Main;
    }
  }
  ~MainMenu() {}
};
#endif  //DND_SRC_UI_MAINMENU_H_
