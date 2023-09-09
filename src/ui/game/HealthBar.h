#ifndef MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
#define MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_

struct HealthBar {
  int delay = 0;
  int width = 50;
  int height = 10;

  HealthBar(int width, int height) noexcept : width(width), height(height) {}
  void draw(int x, int y, const EntityStats& stats) const noexcept {
    const float scaledWidth = width * UI_SCALE;
    const float scaledHeight = height * UI_SCALE;

    const float startX = x - (scaledWidth - width) / 2;
    const float startY = y - scaledHeight - 3;
    const float healthWidth =  stats.health / stats.get_max_health() * scaledWidth - 1;


    DrawRectanglePro(startX + 1, startY + 2, healthWidth, scaledHeight-2, {0, 0}, 0,
                     Colors::Red);
    DrawTexturePro(textures::HEALTH_BAR, {0, 0, 50, 10},
                   {startX, startY, scaledWidth, scaledHeight}, {0, 0}, 0, WHITE);
  }

  void update() { delay--; }
  void hit() { delay = 300; }
};
#endif  //MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
