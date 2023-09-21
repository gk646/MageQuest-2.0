#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
struct XPBar {
  static constexpr float HEIGHT = 20;
  static int prev_req;
  static int next_req;
  static float PLAYER_EXPERIENCE;
  XPBar() { UpdateRequirements(1); }
  static void draw(float x, float y, float size) noexcept {
    DrawRectangleRounded(
        {x, y - HEIGHT, ((PLAYER_EXPERIENCE - prev_req) / next_req) * size - 6, 14}, 2.5F,
        ROUND_SEGMENTS, Colors::questMarkerYellow);
    DrawTextureProFast(textures::ui::skillbar::xpbar, x - 2, y - HEIGHT, 0, WHITE);
  }
  static void update() noexcept {
    std::cout << PLAYER_STATS.level << std::endl;
    if (PLAYER_EXPERIENCE > next_req) {
      LevelUP();
    }
  }
  static inline void UpdateRequirements(int level) noexcept {
    prev_req = GetXPRequiredForLevel(level);
    next_req = GetXPRequiredForLevel(level + 1);
  }

  inline static void AddPlayerExperience(float val) noexcept {
    PLAYER_EXPERIENCE += val * (1 + PLAYER_STATS.effects[XP_MODIFIER_P]);
    if (PLAYER_EXPERIENCE > next_req) {
      LevelUP();
    }
  }
  void PrintEnemiesToKillForLevels(int maxLevel) {
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

 private:
  inline static int GetXPRequiredForLevel(int level) noexcept {
    if (level == 1) return 0;
    return (5 * level) * std::sqrt(level) * std::pow(1.05, level);
  }
  static inline void LevelUP() noexcept {
    PLAYER_STATS.level++;
    UpdateRequirements(PLAYER_STATS.level + 1);
    //TODO animation
  }
};
int XPBar::prev_req = 0;
int XPBar::next_req = 0;
float XPBar::PLAYER_EXPERIENCE = 5;
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_XPBAR_H_
