#ifndef MAGE_QUEST_SRC_UI_SERVERBROWSER_H_
#define MAGE_QUEST_SRC_UI_SERVERBROWSER_H_

#include <raylib.h>
#include <SFML/Network.hpp>
#include <string>
#include <vector>

struct ServerBrowser {
  const int PORT = 25696;
  const float SERVER_ENTRY_HEIGHT = 50;
  const float BUTTON_WIDTH = 80;
  const float BUTTON_HEIGHT = 30;
  const float MANUAL_JOIN_HEIGHT = 50;
  const int WINDOW_WIDTH = 400;

  std::vector<std::string> hostingIps{};

  static bool isPortOpen(const std::string& ip, int port) {
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect(ip, port, sf::milliseconds(15));
    return (status == sf::Socket::Done);
  }
  void connectToHost() {}

  ServerBrowser() = default;
  void scanNetwork() {
    hostingIps.clear();
    for (int i = 1; i <= 255; ++i) {
      std::string ip = "192.168.2." + std::to_string(i);
      if (isPortOpen(ip, 80)) {
        hostingIps.push_back(ip);
      }
    }
  }
  void draw() noexcept {
    const int windowHeight =
        hostingIps.size() * (SERVER_ENTRY_HEIGHT + 15) + MANUAL_JOIN_HEIGHT * 2;

    const int x = (SCREEN_WIDTH - WINDOW_WIDTH) / 2;
    const int y = (SCREEN_HEIGHT - windowHeight) / 2;

    GuiPanel(Rectangle{(float)x, (float)y, (float)WINDOW_WIDTH, (float)windowHeight},
             "Game Browser");

    int yOffset = 35;

    // Draw Server List
    for (const auto& ip : hostingIps) {
      drawServerEntry(x, y, yOffset, ip, PORT);
      yOffset += SERVER_ENTRY_HEIGHT + 15;
    }

    drawManualJoin(x, y, yOffset);
  }

  void drawServerEntry(int x, int y, int yOffset, const std::string& ip, int port) {
    GuiGroupBox(Rectangle{(float)x, (float)y + yOffset, (float)WINDOW_WIDTH, SERVER_ENTRY_HEIGHT},
                ("IP: " + ip + " Port: " + std::to_string(port)).c_str());

    const int buttonX = x + WINDOW_WIDTH - BUTTON_WIDTH - 10;
    const int buttonY = y + yOffset + (SERVER_ENTRY_HEIGHT - BUTTON_HEIGHT) / 2;
    if (GuiButton(Rectangle{(float)buttonX, (float)buttonY, BUTTON_WIDTH, BUTTON_HEIGHT}, "Join")) {
      // Handle join event
    }
  }
  void drawManualJoin(int x, int y, int yOffset) {
    GuiLabel(Rectangle{(float)x + 10, (float)y + yOffset + 15, 40, 20}, "IP:");

    static char ipInput[64] = {0};  // Made static to persist outside function
    GuiTextBox(Rectangle{(float)x + 50, (float)y + yOffset + 10, 200, 30}, ipInput, 64, true);

    const int buttonX = x + WINDOW_WIDTH - BUTTON_WIDTH - 10;
    if (GuiButton(Rectangle{(float)buttonX, (float)y + yOffset + 10, BUTTON_WIDTH, BUTTON_HEIGHT},
                  "Join")) {
      // Handle manual join event
    }
  }
};
#endif  //MAGE_QUEST_SRC_UI_SERVERBROWSER_H_
