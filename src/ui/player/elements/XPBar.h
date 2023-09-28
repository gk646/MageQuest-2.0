#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
struct XPBar {
  static constexpr float HEIGHT = 20;
  static constexpr float WIDTH = 415;
  bool collision = false;
  RectangleR xp_bar = {2, 2, 2, 13};
  static int prev_req;
  static int next_req;
  static float PLAYER_EXPERIENCE;
  static char tx_buf[15];
  XPBar() { UpdateRequirements(1); }
  void draw(float x, float y) noexcept {
    xp_bar.x = x + SCALE(28);
    xp_bar.y = y - SCALE(13);
    xp_bar.width = ((PLAYER_EXPERIENCE - prev_req) / next_req) * WIDTH,
    DrawRectangleRounded(xp_bar, 2.5F, ROUND_SEGMENTS, Colors::questMarkerYellow);
    DrawTextureProFast(textures::ui::skillbar::xpbar, x - 2, y - HEIGHT, 0, WHITE);
    if (collision) {
      DrawTooltip(MOUSE_POS);
    }
  }
  static inline void DrawTooltip(Vector2 m) noexcept {
    DrawTextureProFast(textures::ui::skillbar::tooltip, m.x - 84, m.y - 72, 0, WHITE);
    sprintf(tx_buf, "%d / %d", (int)PLAYER_EXPERIENCE - prev_req, next_req);
    DrawTextExR(VARNISHED, tx_buf, {m.x - 75, m.y - 55}, 13, 0.5, WHITE);

    sprintf(tx_buf, "Collected: %d%%", (int)((PLAYER_EXPERIENCE - prev_req) * 100.0 / next_req));
    DrawTextExR(VARNISHED, tx_buf, {m.x - 10, m.y - 55}, 13, 0.5, WHITE);

    sprintf(tx_buf, "Total XP: %.1f", PLAYER_EXPERIENCE);
    DrawTextExR(VARNISHED, tx_buf, {m.x - 75, m.y - 30}, 13, 0.5, WHITE);
  }
  void update() noexcept {
    if (PLAYER_EXPERIENCE - prev_req >= next_req) {
      LevelUP();
    }
    xp_bar.width = 416;
    collision = !WINDOW_FOCUSED && CheckCollisionPointRec(MOUSE_POS, xp_bar);
  }

  inline static void AddPlayerExperience(float val) noexcept {
    PLAYER_EXPERIENCE += val * (1 + PLAYER_STATS.effects[XP_MODIFIER_P]);
    if (PLAYER_EXPERIENCE - prev_req >= next_req) {
      LevelUP();
    }
  }
  static void PrintEnemiesToKillForLevels(int maxLevel) {
    int currentXP = 0;

    for (int level = 1; level <= maxLevel; ++level) {
      int nextLevelXP = GetXPRequiredForLevel(level + 1);
      int enemiesToKill =
          std::ceil((nextLevelXP - currentXP) / static_cast<float>(level));

      std::cout << "At level " << level << ", you need to kill " << enemiesToKill
                << " enemies to reach level " << (level + 1) << std::endl;

      currentXP = 0;
    }
  }
  inline static void LoadExperience(float experience) noexcept {
    UpdateRequirements(1);
    PLAYER_EXPERIENCE = experience;
    for (uint_fast32_t i = 0; i < 60; i++) {
      if (PLAYER_EXPERIENCE - prev_req >= next_req) {
        LevelUP();
      }
    }
  }

 private:
  static inline void UpdateRequirements(int level) noexcept {
    prev_req += next_req;
    next_req = GetXPRequiredForLevel(level + 1);
  }
  inline static int GetXPRequiredForLevel(int level) noexcept {
    if (level == 1) return 0;
    return (5 * level) * std::sqrt(level) * std::pow(1.05, level);
  }
  static inline void LevelUP() noexcept {
    PLAYER_STATS.level++;
    UpdateRequirements(PLAYER_STATS.level);
    PLAYER_SPENT_POINTS.LevelUP();
    //TODO animation
  }
};
int XPBar::prev_req = 0;
int XPBar::next_req = 0;
float XPBar::PLAYER_EXPERIENCE = 5;
char XPBar::tx_buf[15] = {0};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
