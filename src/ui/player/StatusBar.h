#ifndef MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_

struct StatusBar {
  static constexpr int HEALTH_WIDTH = 225;
  static constexpr int MANA_WIDTH = 160;
  static constexpr int BEGIN_X = 25;
  static constexpr int BEGIN_Y = 25;
  char health_buffer[30];

  void Draw() noexcept {
    const auto& stats = PLAYER_STATS;

    float health = stats.health;
    float max_health = stats.GetMaxHealth();
    float mana = stats.mana;
    float max_mana = stats.GetMaxMana();
    float shield = stats.shield;
    bool hasShield = (shield > 0);

    // Prepare text buffers
    if (hasShield) {
      snprintf(health_buffer, 30, "%d/%d +%d", (int)health, (int)max_health, (int)shield);
    } else {
      snprintf(health_buffer, sizeof(health_buffer), "%d/%d", (int)health, (int)max_health);
    }
    snprintf(TEXT_BUFFER, sizeof(TEXT_BUFFER), "%d/%d", (int)mana, (int)max_mana);

    // Draw health and mana bars
    DrawRectangleRounded({BEGIN_X + 82, BEGIN_Y + 27, (health / max_health) * HEALTH_WIDTH, 10}, 0.3F, 20, Colors::Red);
    DrawRectangleRounded({BEGIN_X + 82, BEGIN_Y + 57, (mana / max_mana) * MANA_WIDTH, 10}, 0.3F, 20, Colors::Blue);

    // Draw status bar texture
    DrawTextureProFast(textures::ui::STATUS_BAR, BEGIN_X, BEGIN_Y, 0, WHITE);

    if (hasShield) {
      DrawRectangleRounded({BEGIN_X + 82, BEGIN_Y + 27, (shield / stats.effects[MAX_SHIELD]) * HEALTH_WIDTH, 13}, 0.5F, 20, Colors::shield);
    }

    // Draw text
    DrawTextExR(EDIT_UNDO, health_buffer, {BEGIN_X + 160, BEGIN_Y + 15}, 16, 1, Colors::white_smoke);
    DrawTextExR(EDIT_UNDO, TEXT_BUFFER, {BEGIN_X + 130, BEGIN_Y + 45}, 16, 0.2F, Colors::white_smoke);
  }
  void Update() noexcept {}
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_STATUSBAR_H_
