#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_

struct CharacterPanel : public Window {

  CharacterPanel() : Window(400, 500, 25,"Character", KEY_C) {}

  void draw() noexcept {
    draw_window();
  }

  void update() noexcept {
    update_window();

  }

};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
