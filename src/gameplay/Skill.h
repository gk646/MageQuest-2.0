#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
//Metaclass that handles display and activation of projectiles and effects
struct Skill {
  inline static constexpr float SKILL_ICON_SIZE = 50;
  inline static constexpr float TOOL_TIP_WIDTH = 220;
  inline static constexpr float TOOL_TIP_BASE_HEIGHT = 90;
  std::string name;
  std::string description;
  UIHitbox hitbox{SKILL_ICON_SIZE, SKILL_ICON_SIZE};
  SkillStats skillStats;
  DamageStats damageStats;
  const Texture& icon;
  int16_t coolDownUpCounter;
  uint8_t attackAnimation = 0;
  bool from_player;
  Skill(const SkillStats& ability_stats, const DamageStats& damage_stats,
        bool from_player, int attack_animation, const Texture& icon) noexcept
      : damageStats(damage_stats),
        skillStats(ability_stats),
        from_player(from_player),
        attackAnimation(attack_animation),
        icon(icon) {
    coolDownUpCounter = (int16_t)ability_stats.coolDownTicks;
  }
  //Activates the skill
  inline virtual void Activate() = 0;
  inline void Update() noexcept { coolDownUpCounter++; };
  virtual void Draw(float x, float y, float size) noexcept {
    DrawTextureProFast(icon, x, y, 0, WHITE);
    DrawCooldown(x, y, size);
  }
  //TODO proper link to support bar
  static inline void DrawSupportBar(float x, float y, float percent) noexcept {
    DrawRectangleProFast(x - SCALE(2), y - SCALE(12), SCALE(53) * percent, SCALE(7),
                         Colors::SUPPORT_BAR_ORANGE);
  }
  //Returns true if skill is ready to use
  [[nodiscard]] inline bool IsUsable() const noexcept {
    return PLAYER_STATS.skill_useable(skillStats, coolDownUpCounter);
  }
  //Handles logic for when the skill is used
  inline void TriggerSkill() noexcept {
    coolDownUpCounter = 0;
    PLAYER.flip = MOUSE_POS.x < CAMERA_X;
    PLAYER.spriteCounter = 0;
    PLAYER.actionState = attackAnimation;
    PLAYER_STATS.UseSkill(skillStats);
  }
  //Does a ray-cast and range-check from player to mouse position
  [[nodiscard]] inline bool RangeLineOfSightCheck() const noexcept {
    Point targetPos = {PLAYER_X + MOUSE_POS.x - CAMERA_X,
                       PLAYER_Y + MOUSE_POS.y - CAMERA_Y};
    if (Point(PLAYER_X + (float)PLAYER.size.x / 2.0F,
              PLAYER_Y + (float)PLAYER.size.y / 2.0F)
            .dist(targetPos) <= (float)skillStats.range) {
      if (PathFinding::LineOfSightCheck(PLAYER.tile_pos, targetPos)) {
        return true;
        //TODO quick notifications
      } else {
        // No line of sight to target
      }
    } else {
      // Target is out of range
    }
    return false;
  }
  //Returns a ptr to a new skill with the given stats / Projectile type is used to identify skills
  inline static Skill* GetNewSkill(ProjectileType type, const SkillStats& stats) noexcept;
  inline void DrawTooltip(float x, float y) noexcept {
    if (hitbox.Update(x, y)) {
      DrawToolTipImpl(x, y);
    }
  }

 private:
  void DrawCooldown(float x, float y, float size) const noexcept {
    int rcd = PLAYER_STATS.GetRemainingCD(skillStats);
    if (coolDownUpCounter < rcd) {
      float side1, side2, side3, side4, side5;
      float coolDownCoefficient;
      coolDownCoefficient = coolDownUpCounter * (size * 4 / rcd);
      side1 = size / 2;
      side2 = 0;
      side3 = 0;
      side4 = 0;
      side5 = 0;
      if (coolDownCoefficient > 0) side1 += coolDownCoefficient;
      if (coolDownCoefficient > size / 2) side2 += coolDownCoefficient - size / 2;
      if (coolDownCoefficient > size * 1.5F) side3 += coolDownCoefficient - size * 1.5F;
      if (coolDownCoefficient > size * 2.5F) side4 += coolDownCoefficient - size * 2.5F;
      if (coolDownCoefficient > size * 3.5F) side5 += coolDownCoefficient - size * 3.5F;

      for (int i = side1; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + i, y,
                 Colors::mediumLightGreyTransparent);
      }
      for (int i = side2; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + size, y + i,
                 Colors::mediumLightGreyTransparent);
      }
      for (int i = side3; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + size - i, y + size,
                 Colors::mediumLightGreyTransparent);
      }
      for (int i = side4; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x, y + size - i,
                 Colors::mediumLightGreyTransparent);
      }
      for (int i = side5; i <= size / 2; i++) {
        DrawLine(x + size / 2, y + size / 2, x + i, y,
                 Colors::mediumLightGreyTransparent);
      }
    }
  }
  void DrawToolTipImpl(float x, float y) noexcept {
    DrawRectangleProFast(x, y, SKILL_ICON_SIZE, SKILL_ICON_SIZE,
                         Colors::lightGreyMiddleAlpha);
    DrawRangeCircle();
    int lineBreaks = 0;
    auto descriptionText = Util::WrapText(
        Util::CreateToolTipString(description, PLAYER_STATS.GetAbilityDmg(damageStats),
                                  skillStats.specialVal1, skillStats.specialVal2),
        TOOL_TIP_WIDTH, MINECRAFT_REGULAR, 15, &lineBreaks);

    float toolTipHeight = TOOL_TIP_BASE_HEIGHT + lineBreaks * 15;
    float startX = MOUSE_POS.x - TOOL_TIP_WIDTH / 2;
    float startY = MOUSE_POS.y - toolTipHeight - 3;
    DrawRectangleRounded({startX, startY, TOOL_TIP_WIDTH, toolTipHeight}, 0.1F,
                         ROUND_SEGMENTS, Colors::mediumLightGrey);
    DrawRectangleRoundedLines({startX, startY, TOOL_TIP_WIDTH, toolTipHeight}, 0.1F,
                              ROUND_SEGMENTS, 2, damageTypeToColor[damageStats.dmgType]);

    //-----------Name-----------//
    DrawTextExR(MINECRAFT_BOLD, name.c_str(), {startX + 5, startY + 5}, 20, 0.5F,
                damageTypeToColor[damageStats.dmgType]);

    //-------------ResourceCost---------------//
    if (skillStats.manaCost > 0) {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%i Mana", (int)skillStats.manaCost);
      DrawTextExR(MINECRAFT_REGULAR, TEXT_BUFFER, {startX + 5, startY + 35}, 15, 0.5F,
                  Colors::darkBackground);
    }

    //-----------CastTime-----------//

    if (skillStats.castTime == 0) {
      DrawTextExR(MINECRAFT_REGULAR, "Instant", {startX + 5, startY + 53}, 15, 0.5F,
                  Colors::darkBackground);
    } else {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%f sec cast", skillStats.castTime / 60.0F);
      DrawTextExR(MINECRAFT_REGULAR, TEXT_BUFFER, {startX + 5, startY + 53}, 15, 0.5F,
                  Colors::darkBackground);
    }

    //-----------CoolDown-----------//

    if (skillStats.coolDownTicks > 0) {
      float cooldownInSeconds = skillStats.coolDownTicks / 60.0F;

      if (cooldownInSeconds == floor(cooldownInSeconds)) {
        snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.0f sec cooldown", cooldownInSeconds);
      } else {
        snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.1f sec cooldown", cooldownInSeconds);
      }

      Util::DrawRightAlignedText(MINECRAFT_REGULAR, 15, TEXT_BUFFER,
                                 startX + TOOL_TIP_WIDTH - 5, startY + 53,
                                 Colors::darkBackground);
    }
    //-----------Description-----------//

    DrawTextExR(MINECRAFT_REGULAR, descriptionText.c_str(), {startX + 5, startY + 75}, 15,
                0.5F, Colors::descriptionOrange);
  }
  inline void DrawRangeCircle() const noexcept {
    if (skillStats.range > 0) {
      DrawTextureScaled(
          textures::ui::skillbar::skillRange,
          {(CAMERA_X + 14) - (float)skillStats.range, (CAMERA_Y + 25) - skillStats.range,
           skillStats.range * 2.05F, skillStats.range * 2.05F},
          0, false, 0, WHITE);
    }
  }
};

#include "skills/Skills.h"

inline static std::array<Skill*, ProjectileType::PROJECTILE_END> SKILLS;
Skill* Skill::GetNewSkill(ProjectileType type, const SkillStats& stats) noexcept {
  switch (type) {
    case POISON_BALL:
      break;
    case FIRE_STRIKE:
    case FIRE_STRIKE_II:
      return new FireStrike_Skill(stats);
    case FIRE_BALL:
      return new FireBall_Skill(stats);
    case BLAST_HAMMER:
      return new BlastHammer_Skill(stats);
    case ENERGY_SPHERE:
      return new EnergySphere_Skill(stats);
    case FIRE_SWORD:
      break;
    case FROST_NOVA:
      return new FrostNova_Skill(stats);
    case ICE_LANCE:
      break;
    case INFERNO_RAY:
      break;
    case LIGHTNING:
      return new Lightning_Skill(stats);
    case PYRO_BLAST:
      break;
    case SOLAR_FLARE:
      break;
    case THUNDER_SPLASH:
      break;
    case THUNDER_STRIKE:
      break;
    case VOID_ERUPTION:
      break;
    case VOID_FIELD:
      break;
    case PROJECTILE_END:
      return nullptr;
    case ARROW_NORMAL:
      break;
    case ARCANE_BOLT:
      return new ArcaneBolt_Skill(stats);
  }
}
inline static void Multiplayer::HandleProjectile(UDP_Projectile* data,
                                                 const Entity* ptr) noexcept {
  switch (data->p_type) {
    case FIRE_BALL: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE,
          SKILLS[FIRE_BALL]->skillStats.lifeSpan, SKILLS[FIRE_BALL]->skillStats.speed,
          data->damage, HitType::ONE_HIT, {nullptr, nullptr, nullptr}, data->pov,
          {data->move_x, data->move_y}, ptr));
      break;
    }
    case FIRE_STRIKE: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE,
          SKILLS[FIRE_STRIKE]->skillStats.lifeSpan, SKILLS[FIRE_STRIKE]->skillStats.speed,
          data->damage, HitType::CONTINUOUS, {nullptr, nullptr, nullptr}, data->pov,
          {data->move_x, data->move_y}, ptr));
      break;
    }
    case BLAST_HAMMER: {
      PROJECTILES.emplace_back(
          new BlastHammer({(float)data->x, (float)data->y}, !FRIENDLY_FIRE,
                          SKILLS[BLAST_HAMMER]->skillStats.lifeSpan, 0, data->damage,
                          HitType::ONE_TICK, {nullptr, nullptr, nullptr}, data->pov,
                          {0, 0}, RANGE_01(RNG_RANDOM) > 0.5F, ptr));
      break;
    }
    case ENERGY_SPHERE: {
      PROJECTILES.emplace_back(new EnergySphere(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE,
          SKILLS[ENERGY_SPHERE]->skillStats.lifeSpan,
          SKILLS[ENERGY_SPHERE]->skillStats.speed, data->damage,
          {nullptr, nullptr, nullptr}, {data->move_x, data->move_y}, ptr));
      break;
    }
  }
}
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
