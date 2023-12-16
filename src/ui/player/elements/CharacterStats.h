#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_CHARACTERSTATS_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_CHARACTERSTATS_H_
struct CharacterStats final : public Content {
  inline static constexpr float STAT_FONT_SIZE = 15;
  inline static constexpr float HORIZONTAL_GAP = 170;
  inline static std::string offensive = "Offensive:";
  inline static std::string defensive = "Defensive:";
  inline static std::string resources = "Resources:";
  inline static std::string utility = "Utility:";
  inline static std::string elemental = "Elemental Affinities:";
  inline static RectangleR BOUNDS{0, 0, 200, 450};
  ;

  float lastHeight = 0;
  CharacterStats() = default;
  void Draw(RectangleR bounds, float scrollOffset) noexcept final {
    BOUNDS.x = bounds.x;
    BOUNDS.y = bounds.y;
    UpdateLimits(bounds, scrollOffset);
    DrawRectangleRounded(BOUNDS, 0.1F, GAME_SETTINGS.roundSegments, Colors::LightGrey);
    DrawRectangleRoundedLines(BOUNDS, 0.1F, GAME_SETTINGS.roundSegments, 2, Colors::darkBackground);
    bounds.x += 3;
    DrawOffensive(bounds.x, bounds.y);
    DrawDefensive(bounds.x, bounds.y);
    DrawResources(bounds.x, bounds.y);
    DrawUtility(bounds.x, bounds.y);
    DrawElementalAffinities(bounds.x, bounds.y);
    lastHeight = bounds.y;
  }

  void Update() noexcept final {}
  [[nodiscard]] float GetHeight() const noexcept final { return 580; }
  [[nodiscard]] float GetWidth() const noexcept final { return 200; }

 private:
  void DrawOffensive(float x, float& y) {
    DrawCategory(x, y, offensive,
                 {WEAPON_DAMAGE, ARCANE_DMG_P, DARK_DMG_P, POISON_DMG_P, FIRE_DMG_P,
                  ICE_DMG_P, CRIT_CHANCE, CRIT_DAMAGE_P});
  }
  void DrawDefensive(float x, float& y) {
    DrawCategory(
        x, y, defensive,
        {ARMOUR_MULT_P, HEALTH_MULT_P, DAMAGE_RESISTANCE_P, DODGE_CHANCE, MAX_SHIELD});
  }
  void DrawResources(float x, float& y) {
    DrawCategory(x, y, resources, {MANA_MULT_P, MANA_REGEN_MULT_P, HEALTH_REGEN_MULT_P});
  }
  void DrawUtility(float x, float& y) {
    DrawCategory(x, y, utility, {SPEED_MULT_P, XP_MODIFIER_P});
  }
  void DrawElementalAffinities(float x, float y) {
    DrawCategory(x, y, elemental,
                 {ARCANE_DMG_WEAKNESS_P, DARK_DMG_WEAKNESS_P, POISON_DMG_WEAKNESS_P,
                  FIRE_DMG_WEAKNESS_P, ICE_DMG_WEAKNESS_P, ARCANE_COST_REDUCTION_P,
                  DARK_COST_REDUCTION_P, POISON_COST_REDUCTION_P, FIRE_COST_REDUCTION_P,
                  ICE_COST_REDUCTION_P});
  }
  inline void DrawCategory(float x, float& y, const std::string& name,
                           std::initializer_list<Stat> stats) {
    if (IsInBounds(y, 16)) {
      DrawTextExR(MINECRAFT_BOLD, name.c_str(), {x, y}, 16, 0.5F, Colors::darkBackground);
    }

    y += STAT_FONT_SIZE;
    x += 5;
    for (const auto stat : stats) {
      if (IsInBounds(y, STAT_FONT_SIZE)) {
        snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s:", statToName[stat].c_str());

        DrawTextExR(MINECRAFT_REGULAR, TEXT_BUFFER, {x, y}, STAT_FONT_SIZE, 0.5F,
                    Colors::darkBackground);
        float val = PLAYER_STATS.effects[stat];
        if (IsPercentile(stat)) {
          val *= 100.0F;
        }
        if (!IsPercentStat(stat)) {
          snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%+.2f", val);
        } else {
          snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%+.2f%%", val);
        }
        Util::DrawRightAlignedText(MINECRAFT_BOLD, STAT_FONT_SIZE, TEXT_BUFFER, x + 173,
                                   y, Colors::darkBackground);
      }
      y += STAT_FONT_SIZE;
    }
    y += 3;
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_CHARACTERSTATS_H_
