#ifndef MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
#define MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_

struct HealthBar {
  int16_t delay = 0;
  uint8_t width = 50;
  uint8_t height = 10;
  static char buffer[15];
  explicit HealthBar(int width) noexcept : width(width) {}
  void Draw(float x, float y, const EntityStats& stats) const noexcept {
    if (delay <= 0) return;
    const float scaledWidth = 50 * UI_SCALE;
    const float scaledHeight = height * UI_SCALE;

    const float startX = x - (scaledWidth - width) / 2;
    const float startY = y - scaledHeight * 1.2F;
    const float healthWidth = stats.health / stats.GetMaxHealth() * scaledWidth - 2;

    DrawRectanglePro(startX + 1, startY + 4, healthWidth, scaledHeight / 2, {0, 0}, 0,
                     Colors::Red);
    DrawTexturePro(textures::ui::HEALTH_BAR, {0, 0, 50, 10},
                   {startX, startY, scaledWidth, scaledHeight}, {0, 0}, 0, WHITE);
    if (SHOW_HEALTH_NUMBERS) {
      std::sprintf(buffer, "%.0f/%.0f", stats.health, stats.effects[MAX_HEALTH]);
      DrawTextExR(MINECRAFT_BOLD, buffer,
                  {x - (MeasureTextEx(MINECRAFT_BOLD, buffer, 12, 0.1F).x - width) / 2,
                   startY - height},
                  12, 0.1, WHITE);
    }
  }
  void update() { delay--; }
  void hit() { delay = 300; }
};
char HealthBar::buffer[15];
#endif  //MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
