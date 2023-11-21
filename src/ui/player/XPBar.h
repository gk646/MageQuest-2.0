#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_

struct XPBar {
  static constexpr float HEIGHT = 20.0f;
  static constexpr float WIDTH = 412.0f;
  static constexpr float XP_TOOL_TIP_WIDTH = 225.0f;
  static constexpr float XP_TOOL_TIP_HEIGHT = 70.0f;
  RectangleR xpBarBounds = {2.0f, 2.0f, WIDTH, 13.0f};
  bool collision = false;

  XPBar() = default;

  void Draw(float x, float y) noexcept {
    xpBarBounds.x = x + SCALE(28);
    xpBarBounds.y = y - SCALE(13);
    DrawRectangleRounded(xpBarBounds, 2.5f, ROUND_SEGMENTS, Colors::questMarkerYellow);
    DrawTextureProFast(textures::ui::skillbar::xpbar, x - 2, y - HEIGHT, 0, WHITE);

    if (collision) {
      DrawTooltip(MOUSE_POS);
    }
  }
  void Update() noexcept {
    xpBarBounds.width =
        ((PLAYER_SECOND_STATS.experience - PLAYER_SECOND_STATS.prevLevelReq) /
         PLAYER_SECOND_STATS.nextLevelReq) *
        WIDTH;
    collision = !WINDOW_FOCUSED &&
                CheckCollisionPointRec(
                    MOUSE_POS, {xpBarBounds.x, xpBarBounds.y, WIDTH, xpBarBounds.height});
    if (PLAYER_SECOND_STATS.experience - PLAYER_SECOND_STATS.prevLevelReq >=
        PLAYER_SECOND_STATS.nextLevelReq) {
      PLAYER_SECOND_STATS.LevelUP();
    }
  }
  static void PrintEnemiesToKillForLevels(int maxLevel) {
    int currentXP = 0;
    for (int level = 1; level <= maxLevel; ++level) {
      int nextLevelXP = PLAYER_SECOND_STATS.GetXPRequiredForLevel(level + 1);
      int enemiesToKill =
          std::ceil((nextLevelXP - currentXP) / static_cast<float>(level));

      std::cout << "At level " << level << ", you need to kill " << enemiesToKill
                << " enemies to reach level " << (level + 1) << std::endl;

      currentXP = nextLevelXP;
    }
  }

 private:
  static void DrawTooltip(Vector2 m) noexcept {
    m.x -= XP_TOOL_TIP_WIDTH / 2.0f;
    m.y -= XP_TOOL_TIP_HEIGHT + 10.0f;

    DrawTextureScaled(textures::ui::skillbar::tooltip,
                      {m.x, m.y, XP_TOOL_TIP_WIDTH, XP_TOOL_TIP_HEIGHT}, false, false, 0,
                      WHITE);
    DrawXPText(m);
  }
  static void DrawXPText(Vector2 m) noexcept {
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%d / %d",
             static_cast<int>(PLAYER_SECOND_STATS.experience -
                              PLAYER_SECOND_STATS.prevLevelReq),
             PLAYER_SECOND_STATS.nextLevelReq);
    DrawTextExR(MINECRAFT_BOLD, TEXT_BUFFER, {m.x + 10, m.y + 15}, 15, 0.5, WHITE);

    snprintf(
        TEXT_BUFFER, TEXT_BUFFER_SIZE, "Collected: %d%%",
        static_cast<int>((PLAYER_SECOND_STATS.experience -
                          static_cast<float>(PLAYER_SECOND_STATS.prevLevelReq)) *
                         100.0f / static_cast<float>(PLAYER_SECOND_STATS.nextLevelReq)));
    Util::DrawRightAlignedText(MINECRAFT_BOLD, 15, TEXT_BUFFER,
                               m.x + XP_TOOL_TIP_WIDTH - 10, m.y + 15, WHITE);

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Total XP: %.1f",
             PLAYER_SECOND_STATS.experience);
    Util::DrawCenteredText(MINECRAFT_BOLD, 16, TEXT_BUFFER,
                           m.x + XP_TOOL_TIP_WIDTH / 2.0f, m.y + 35, WHITE);
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
