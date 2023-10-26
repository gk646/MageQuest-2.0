
#include "Includes.h"



int main() {
  if (!SteamAPI_Init()) {
    exit(1);
  }
  SteamNetworkingUtils()->InitRelayNetworkAccess();
  DataBaseHandler::Init();
  auto game = new Game();
  game->start();
  delete game;
  return 0;
}
