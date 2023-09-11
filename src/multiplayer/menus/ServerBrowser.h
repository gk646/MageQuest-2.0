#ifndef MAGE_QUEST_SRC_UI_SERVERBROWSER_H_
#define MAGE_QUEST_SRC_UI_SERVERBROWSER_H_

struct ServerBrowser {
  const float SERVER_ENTRY_HEIGHT = 50;
  const float BUTTON_WIDTH = 80;
  const float BUTTON_HEIGHT = 30;
  const float MANUAL_JOIN_HEIGHT = 50;
  const int WINDOW_WIDTH = 400;
  std::string ip_field;

  ServerBrowser(){
    ip_field = std::string(30, ' ');
  };

  void draw() noexcept {
    const int windowHeight = 0 * (SERVER_ENTRY_HEIGHT + 15) + MANUAL_JOIN_HEIGHT * 2;

    const int x = (SCREEN_WIDTH - WINDOW_WIDTH) / 2;
    const int y = (SCREEN_HEIGHT - windowHeight) / 2;

    GuiPanel(RectangleR{(float)x, (float)y, (float)WINDOW_WIDTH, (float)windowHeight},
             "Game Browser");

    int yOffset = 35;

    drawManualJoin(x, y, yOffset);
  }

  void drawServerEntry(int x, int y, int yOffset, const std::string& ip, int port) {
    GuiGroupBox(RectangleR{(float)x, (float)y + yOffset, (float)WINDOW_WIDTH,
                           SERVER_ENTRY_HEIGHT},
                ("IP: " + ip + " Port: " + std::to_string(port)).c_str());

    const int buttonX = x + WINDOW_WIDTH - BUTTON_WIDTH - 10;
    const int buttonY = y + yOffset + (SERVER_ENTRY_HEIGHT - BUTTON_HEIGHT) / 2;
    if (GuiButton(RectangleR{(float)buttonX, (float)buttonY, BUTTON_WIDTH, BUTTON_HEIGHT},
                  "Join")) {
      // Handle join event
    }
  }
  void drawManualJoin(int x, int y, int yOffset) {
    GuiLabel(RectangleR{(float)x + 10, (float)y + yOffset + 15, 40, 20}, "IP:");

    GuiTextBox(RectangleR{(float)x + 50, (float)y + yOffset + 10, 200, 30},
               ip_field.data(), 64, true);

    const int buttonX = x + WINDOW_WIDTH - BUTTON_WIDTH - 10;
    if (GuiButton(RectangleR{(float)buttonX, (float)y + yOffset + 10, BUTTON_WIDTH,
                             BUTTON_HEIGHT},
                  "Join")) {
      ip_field.erase(std::remove(ip_field.begin(), ip_field.end(), ' '), ip_field.end());
      Client::init(ip_field.c_str());
      GAME_STATE = GameState::Game;
      StopSound(sound::intro);
    }
  }
};
#endif  //MAGE_QUEST_SRC_UI_SERVERBROWSER_H_
