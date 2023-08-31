#ifndef DND_SRC_UI_SERVERBROWSER_H_
#define DND_SRC_UI_SERVERBROWSER_H_

#include <raylib.h>
#include <SFML/Network.hpp>
#include <string>
#include <vector>
#include "../Component.h"

struct ServerBrowser : public Component {
  int PORT = 25696;
  int SERVER_ENTRY_HEIGHT = 50;
  int BUTTON_WIDTH = 80;
  int BUTTON_HEIGHT = 30;
  int MANUAL_JOIN_HEIGHT = 50;

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
  void draw(float ui, int screen_width, int screen_height) final {
    int windowWidth = 400;
    int windowHeight = hostingIps.size() * (SERVER_ENTRY_HEIGHT + 15) + MANUAL_JOIN_HEIGHT * 2;

    int x = (screen_width - windowWidth) / 2;
    int y = (screen_height - windowHeight) / 2;

    GuiPanel(Rectangle{(float)x, (float)y, (float)windowWidth, (float)windowHeight},
             "Game Browser");

    int yOffset = 35;
    for (const auto& ip : hostingIps) {

      GuiGroupBox(Rectangle{(float)x, (float)y + yOffset, (float)windowWidth, SERVER_ENTRY_HEIGHT},
                  ("IP: " + ip + " Port: " + std::to_string(PORT)).c_str());

      // Drawing the 'Join' button for this server
      int buttonX = x + windowWidth - BUTTON_WIDTH - 10;
      int buttonY = y + yOffset + (SERVER_ENTRY_HEIGHT - BUTTON_HEIGHT) / 2;
      if (GuiButton(Rectangle{(float)buttonX, (float)buttonY, BUTTON_WIDTH, BUTTON_HEIGHT},
                    "Join")) {
        // Handle the join event for this IP here
      }

      yOffset += SERVER_ENTRY_HEIGHT + 15;
    }

    // Manual Join Section
    int manualY = y + yOffset;
    GuiLabel(Rectangle{(float)x + 10, (float)manualY + 15, 40, 20}, "IP:");
    static char ipInput[64] = {
        0};  // This should persist outside of function, made static for simplicity
    GuiTextBox(Rectangle{(float)x + 50, (float)manualY + 10, 200, 30}, ipInput, 64, true);

    int joinButtonX = x + windowWidth - BUTTON_WIDTH - 10;
    if (GuiButton(Rectangle{(float)joinButtonX, (float)manualY + 10, BUTTON_WIDTH, BUTTON_HEIGHT},
                  "Join")) {}
  }
};
#endif  //DND_SRC_UI_SERVERBROWSER_H_
