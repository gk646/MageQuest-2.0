#ifndef MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_

struct StatusBar {
  static constexpr int health_width = 228;
  static constexpr int mana_width = 160;
  static constexpr int beginX = 25;
  static constexpr int beginY = 25;

  void draw() const noexcept {
    DrawRectanglePro(beginX + 82, beginY + 27,
                     (PLAYER_STATS.general.health / PLAYER_STATS.general.max_health) * health_width,
                     10, {0, 0}, 0, Colors::Red);
    DrawRectanglePro(beginX + 82, beginY + 57,
                     (PLAYER_STATS.general.mana / PLAYER_STATS.general.max_mana) * mana_width, 10,
                     {0, 0}, 0, Colors::Blue);
    DrawTextureProFast(textures::STATUS_BAR, beginX, beginY, 0, WHITE);
  }

  void update() noexcept {}
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
