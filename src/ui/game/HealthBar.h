#ifndef MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
#define MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_



struct HealthBar {
  int delay = 300;
  int active_ticks = 0;
  bool show = false;

  int width;
  int height;

  HealthBar(int width, int height) noexcept : width(width), height(height) {}
  void draw(int x, int y, const EntityStats& stats) const noexcept {
    DrawRectangleLines(x - (width * UI_SCALE - width) / 2, y - height * UI_SCALE, width * UI_SCALE,
                       height * UI_SCALE, BLUE);
    DrawRectanglePro(x - (width * UI_SCALE - width) / 2, y - height * UI_SCALE,
                     (stats.health / stats.get_max_health()) * width * UI_SCALE,
                     height * UI_SCALE, {0, 0}, 0, BLACK);
  }
  void update() {
    if (show) {
      active_ticks++;
      if (active_ticks >= delay) {
        show = false;
        active_ticks = 0;
      }
    }
  }
  void hit() {
    active_ticks = 0;
    show = true;
  }
};
#endif  //MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
