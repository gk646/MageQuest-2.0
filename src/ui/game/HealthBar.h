#ifndef MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
#define MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_

struct HealthBar {
  int16_t delay = 0;
  uint8_t width = 50;
  uint8_t height = 10;
  static char buffer[15];
  HealthBar(int width, int height) noexcept : width(width), height(height) {}
  void draw(float x, float y, const EntityStats& stats) const noexcept {
    const float scaledWidth = width * UI_SCALE;
    const float scaledHeight = height * UI_SCALE;

    const float startX = x - (scaledWidth - width) / 2;
    const float startY = y - scaledHeight - 3;
    const float healthWidth =  stats.health / stats.get_max_health() * scaledWidth - 1;

    std::sprintf(buffer,"%.0f/%.0f",stats.health,stats.effects[MAX_HEALTH]);
    DrawRectanglePro(startX + 1, startY + 2, healthWidth, scaledHeight-2, {0, 0}, 0,
                     Colors::Red);
    DrawTexturePro(textures::ui::HEALTH_BAR, {0, 0, 50, 10},
                   {startX, startY, scaledWidth, scaledHeight}, {0, 0}, 0, WHITE);
    DrawTextExR(MINECRAFT_BOLD,buffer,{startX+scaledWidth/2- GetTextWidth(buffer)/2,startY-height},15,0.5,WHITE);
  }
  void update() { delay--; }
  void hit() { delay = 10; }
};
char  HealthBar::buffer[15];
#endif  //MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
