#include "Game.h"

int main() {
  auto game = new Game();
  game->start();
  delete game;
  return 0;
}
