#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_

struct XPBar {
  inline static constexpr float HEIGHT = 20;
  inline static constexpr float WIDTH = 415;
  inline static constexpr float XP_TOOL_TIP_WIDTH = 225;
  inline static constexpr float XP_TOOL_TIP_HEIGHT = 70;
  bool collision = false;
  RectangleR xp_bar = {2, 2, 2, 13};
  static int prev_req;
  static int next_req;
  static float PLAYER_EXPERIENCE;
  XPBar() { UpdateRequirements(1); }
  void Draw(float x, float y) noexcept {
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
    //normalizing values to top left
    m.x -= XP_TOOL_TIP_WIDTH / 2.0F;
    m.y -= XP_TOOL_TIP_HEIGHT + 10;
    DrawTextureScaled(textures::ui::skillbar::tooltip,
                      {m.x, m.y, XP_TOOL_TIP_WIDTH, XP_TOOL_TIP_HEIGHT}, false, false, 0,
                      WHITE);
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%d / %d", (int)PLAYER_EXPERIENCE - prev_req,
              next_req);
    DrawTextExR(MINECRAFT_BOLD, TEXT_BUFFER, {m.x + 10, m.y + 15}, 15, 0.5, WHITE);

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Collected: %d%%",
              (int)((PLAYER_EXPERIENCE - prev_req) * 100.0 / next_req));
    Util::DrawRightAlignedText(MINECRAFT_BOLD, 15, TEXT_BUFFER,
                               m.x + XP_TOOL_TIP_WIDTH - 10, m.y + 15, WHITE);

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Total XP: %.1f", PLAYER_EXPERIENCE);
    Util::DrawCenteredText(MINECRAFT_BOLD, 16, TEXT_BUFFER,
                           m.x + XP_TOOL_TIP_WIDTH / 2.0F, m.y + 35, WHITE);
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
  //Called on a level up event
  static inline void LevelUP() noexcept {
    PLAYER_STATS.level++;
    UpdateRequirements(PLAYER_STATS.level);
    PLAYER_SPENT_POINTS.LevelUP();
    PLAYER_STATS.ReCalculatePlayerStats();
    //TODO animation
  }
};
int XPBar::prev_req = 0;
int XPBar::next_req = 0;
float XPBar::PLAYER_EXPERIENCE = 5;
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
