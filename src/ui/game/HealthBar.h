#ifndef MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
#define MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_

struct HealthBar {
  int16_t delay = 0;
  uint8_t width = 50;
  uint8_t height = 10;
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
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.0f/%.0f", stats.health, stats.effects[MAX_HEALTH]);
      Util::DrawCenteredText(MINECRAFT_BOLD, 12, TEXT_BUFFER, x - width / 2,
                             startY - height, WHITE);
    }
  }
  void update() { delay--; }
  void Update() { delay = 300; }
};
#endif  //MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
