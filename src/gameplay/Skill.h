#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILL_H_

struct Skill {
  inline static constexpr float SKILL_ICON_SIZE = 50;
  inline static constexpr float TOOL_TIP_WIDTH = 150;
  inline static constexpr float TOOL_TIP_HEIGHT = 200;
  std::string name;
  UIHitbox hitbox{SKILL_ICON_SIZE, SKILL_ICON_SIZE};
  SkillStats skill_stats;
  DamageStats damage_stats;
  const Texture& icon;
  int cool_down_ticks;
  int attack_animation = 0;
  bool from_player;
  Skill(const SkillStats& ability_stats, const DamageStats& damage_stats,
        bool from_player, int attack_animation, const Texture& icon) noexcept
      : damage_stats(damage_stats),
        skill_stats(ability_stats),
        from_player(from_player),
        attack_animation(attack_animation),
        icon(icon) {
    cool_down_ticks = (int)ability_stats.cool_down;
  }
  inline virtual void activate() = 0;
  inline void Update() noexcept { cool_down_ticks++; };
  virtual void Draw(float x, float y, float size) noexcept {
    DrawTextureProFast(icon, x, y, 0, WHITE);
    DrawCooldown(x, y, size);
    if (hitbox.Update(x, y)) {
      DrawToolTip(x, y);
    }
  }
  static inline void DrawSupportBar(float x, float y, float percent) noexcept {
    DrawRectangleProFast(x - SCALE(2), y - SCALE(12), SCALE(53) * percent, SCALE(7),
                         Colors::SUPPORT_BAR_ORANGE);
  }
  [[nodiscard]] inline bool IsUsable() const noexcept {
    return PLAYER_STATS.skill_useable(skill_stats, cool_down_ticks);
  }
  inline void TriggerSkill() noexcept {
    cool_down_ticks = 0;
    PLAYER.flip = MOUSE_POS.x < CAMERA_X;
    PLAYER.sprite_counter = 0;
    PLAYER.action_state = attack_animation;
    PLAYER_STATS.UseSkill(skill_stats);
  }
  [[nodiscard]] inline bool RangeLineOfSightCheck() const noexcept {
    Point targetPos = {PLAYER_X + MOUSE_POS.x - CAMERA_X,
                       PLAYER_Y + MOUSE_POS.y - CAMERA_Y};
    if (PLAYER.pos.dist(targetPos) <= skill_stats.range) {
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

 private:
  void DrawCooldown(float x, float y, float size) const noexcept {
    int rcd = PLAYER_STATS.GetRemainingCD(skill_stats);
    if (cool_down_ticks < rcd) {
      float side1, side2, side3, side4, side5;
      float coolDownCoefficient;
      coolDownCoefficient = cool_down_ticks * (size * 4 / rcd);
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
  void DrawToolTip(float x, float y) noexcept {
    DrawRectangleProFast(x, y, SKILL_ICON_SIZE, SKILL_ICON_SIZE,
                         Colors::lightGreyMiddleAlpha);
    DrawRangeCircle();
    DrawRectangleRounded({MOUSE_POS.x - TOOL_TIP_WIDTH, MOUSE_POS.y - TOOL_TIP_HEIGHT,
                          TOOL_TIP_WIDTH, TOOL_TIP_HEIGHT},
                         0.2F, ROUND_SEGMENTS, Colors::mediumLightGrey);
    DrawRectangleRoundedLines(
        {MOUSE_POS.x - TOOL_TIP_WIDTH, MOUSE_POS.y - TOOL_TIP_HEIGHT, TOOL_TIP_WIDTH,
         TOOL_TIP_HEIGHT},
        0.2F, ROUND_SEGMENTS, 2, Colors::darkBackground);




  }
  inline void DrawRangeCircle() const noexcept {
    if (skill_stats.range > 0) {
      DrawTextureScaled(textures::ui::skillbar::skillRange,
                        {CAMERA_X - skill_stats.range, CAMERA_Y - skill_stats.range,
                         skill_stats.range * 2.0F, skill_stats.range * 2.0F},
                        0, false, 0, WHITE);
    }
  }
};

#include "skills/Dummy.h"
#include "skills/FireBall_Skill.h"
#include "skills/BlastHammer_Skill.h"
#include "skills/EnergySphere_Skill.h"
#include "skills/FireStrike_Skill.h"

inline static void Multiplayer::HandleProjectile(UDP_Projectile* data,
                                                 const Entity* ptr) noexcept {
  switch (data->p_type) {
    case FIRE_BALL: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, FireBall_Skill::LIFE_SPAN,
          FireBall_Skill::SPEED, data->damage, HitType::ONE_HIT, {}, data->pov,
          {data->move_x, data->move_y}, ptr));
      break;
    }
    case FIRE_STRIKE: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, FireStrike_Skill::LIFE_SPAN,
          FireStrike_Skill::SPEED, data->damage, HitType::CONTINUOUS, {}, data->pov,
          {data->move_x, data->move_y}, ptr));
      break;
    }
    case BLAST_HAMMER: {
      PROJECTILES.emplace_back(new BlastHammer(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, BlastHammer_Skill::LIFE_SPAN,
          0, data->damage, HitType::ONE_TICK, {}, data->pov, {0, 0},
          RANGE_01(RNG_RANDOM) > 0.5F, ptr));
      break;
    }
    case ENERGY_SPHERE: {
      PROJECTILES.emplace_back(new EnergySphere(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, EnergySphere_Skill::LIFESPAN,
          EnergySphere_Skill::SPEED, data->damage, HitType::CONTINUOUS, {},
          {data->move_x, data->move_y}, ptr));
      break;
    }
  }
}
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
