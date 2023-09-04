#ifndef MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_

struct StatusBar {

  void draw() {

    DrawTextureProFast(textures::STATUS_BAR, 25, 25,0, WHITE); }

  void update() noexcept {

  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
