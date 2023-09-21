#ifndef MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_

struct StatusBar {
  static constexpr int health_width = 225;
  static constexpr int mana_width = 160;
  static constexpr int beginX = 25;
  static constexpr int beginY = 25;
  char health_buffer[30];
  char mana_buffer[30];

  void draw() noexcept {
    const auto& stats = PLAYER_STATS;

    float health = stats.health;
    float max_health = stats.get_max_health();
    float mana = stats.mana;
    float max_mana = stats.get_max_mana();
    float shield = stats.shield;

    if (shield > 0) {
      snprintf(health_buffer, 30, "%d/%d +%d", (int)health, (int)max_health, (int)shield);

      snprintf(mana_buffer, 30, "%d/%d", (int)mana, (int)max_mana);
      DrawRectangleRounded(
          {beginX + 82, beginY + 27, (health / max_health) * health_width, 10}, 0.3F, 20,
          Colors::Red);
      DrawRectangleRounded({beginX + 82, beginY + 57, (mana / max_mana) * mana_width, 10},
                           0.3F, 20, Colors::Blue);
      DrawTextureProFast(textures::ui::STATUS_BAR, beginX, beginY, 0, WHITE);
      DrawRectangleRounded({beginX + 82, beginY + 27,
                            (shield / stats.effects[MAX_SHIELD]) * health_width, 13},
                           0.5F, 20, Colors::shield);
      DrawTextExR(EDIT_UNDO, health_buffer, {beginX + 160, beginY + 15}, 16, 1,
                 Colors::white_smoke);
      DrawTextExR(EDIT_UNDO, mana_buffer, {beginX + 130, beginY + 45}, 16, 0.2,
                 Colors::white_smoke);
    } else {
      snprintf(health_buffer, sizeof(health_buffer), "%d/%d", (int)health,
               (int)max_health);
      snprintf(mana_buffer, sizeof(mana_buffer), "%d/%d", (int)mana, (int)max_mana);
      DrawRectanglePro(beginX + 82, beginY + 27, (health / max_health) * health_width, 10,
                       {0, 0}, 0, Colors::Red);
      DrawRectanglePro(beginX + 82, beginY + 57, (mana / max_mana) * mana_width, 10,
                       {0, 0}, 0, Colors::Blue);
      DrawTextureProFast(textures::ui::STATUS_BAR, beginX, beginY, 0, WHITE);
      DrawTextExR(EDIT_UNDO, health_buffer, {beginX + 160, beginY + 15}, 16, 1,
                 Colors::white_smoke);
      DrawTextExR(EDIT_UNDO, mana_buffer, {beginX + 130, beginY + 45}, 16, 0.2,
                 Colors::white_smoke);
    }
  }

  void update() noexcept {}
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
