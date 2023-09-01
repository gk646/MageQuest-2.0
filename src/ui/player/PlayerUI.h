#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

struct PlayerUI {

  HotBar hot_bar_{5, 1};
  void draw() const { hot_bar_.draw(); }

  void update() { hot_bar_.update(); }
};
#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
