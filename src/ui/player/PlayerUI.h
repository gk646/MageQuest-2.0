#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

#include "StatusBar.h"

struct PlayerUI {
  MiniMap mini_map{};
  CharacterPanel char_panel{};
  StatusBar status_bar{};
  void draw() {
    PLAYER.status_effects.draw();
    PLAYER_HOTBAR.draw();
    status_bar.draw();
    mini_map.draw();
    char_panel.draw();
  }

  void update() noexcept {
    status_bar.update();
    char_panel.update();
    PLAYER_HOTBAR.update();
  }
};
#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
