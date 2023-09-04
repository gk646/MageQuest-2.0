#ifndef MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
#define MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_

struct PlayerUI {
  MiniMap mini_map{};
  CharacterPanel char_panel{};
  void draw() {
    PLAYER.status_effects.draw();
    PLAYER_HOTBAR.draw();
    mini_map.draw();
    char_panel.draw();
  }

  void update() noexcept {
    char_panel.update();
    PLAYER_HOTBAR.update();
  }
};
#endif  //MAGE_QUEST_SRC_UI_PLAYER_PLAYERUI_H_
