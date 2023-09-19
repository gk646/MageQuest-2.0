
#include "Includes.h"



int main() {
  if (!SteamAPI_Init()) {
    exit(1);
  }
  SteamNetworkingUtils()->InitRelayNetworkAccess();
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(TARGET_FPS);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 21);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mage Quest II");
  InitAudioDevice();
  SetExitKey(0);
  auto game = new Game();
  game->start();
  delete game;
  return 0;
}
