#ifndef DUNGEONM_SRC_UI_GAME_HEALTHBAR_H_
#define DUNGEONM_SRC_UI_GAME_HEALTHBAR_H_

#include "../../gameobjects/entities/attributes/Stats.h"
#include "../../util/GlobalVariables.h"
#include "../Component.h"

struct HealthBar {
  int delay = 300;
  int active_ticks;
  bool show = false;

  int width;
  int height;

  HealthBar(int width, int height) : width(width), height(height) {}
  void draw(int x, int y, const EntityStats& stats) const {
    DrawRectangleLines(x - (width * UI_SCALE - width) / 2, y - height * UI_SCALE, width * UI_SCALE,
                       height * UI_SCALE, BLUE);
    DrawRectanglePro(
        {x - (width * UI_SCALE - width) / 2, y - height * UI_SCALE,
         (stats.general.health / stats.general.max_health) * width * UI_SCALE, height * UI_SCALE},
        {0, 0}, 0, BLACK);
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
#endif  //DUNGEONM_SRC_UI_GAME_HEALTHBAR_H_
