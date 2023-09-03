#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

struct PlayerUI {

  void draw() const {
    PLAYER.status_effects.draw();
    PLAYER_HOTBAR.draw();
  }

  void update() {}
};
#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
