#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILL_H_

struct Skill {
  DamageStats damage_stats;
  SkillStats skill_stats;
  int cool_down_ticks = 0;
  bool from_player;
  int attack_animation = 0;
  // Texture2D texture;
  Skill(SkillStats ability_stats, DamageStats damage_stats, bool from_player,
        int attack_animation) noexcept
      : damage_stats(damage_stats),
        skill_stats(ability_stats),
        from_player(from_player),
        attack_animation(attack_animation) {
    cool_down_ticks = ability_stats.cool_down;
  }
  inline virtual void activate() = 0;
  inline void update() noexcept { cool_down_ticks++; };
  void draw(float x, float y, float size) const noexcept {
    DrawRectanglePro(x, y, size, size, {0, 0}, 0, RED);
    if (!PLAYER_STATS.skill_useable(skill_stats, cool_down_ticks)) {
      float side1, side2, side3, side4, side5;
      float coolDownCoefficient;
      coolDownCoefficient = cool_down_ticks * (size * 4 / skill_stats.cool_down);
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
        DrawLine(x + size / 2, y + size / 2, x + i, y, Colors::mediumVeryLight);
      }
      for (int i = side2; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + size, y + i, Colors::mediumVeryLight);
      }
      for (int i = side3; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + size - i, y + size,
                 Colors::mediumVeryLight);
      }
      for (int i = side4; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x, y + size - i, Colors::mediumVeryLight);
      }
      for (int i = side5; i <= size / 2; i++) {
        DrawLine(x + size / 2, y + size / 2, x + i, y, Colors::mediumVeryLight);
      }
    }
  }
  [[nodiscard]] inline bool use_able() const noexcept {
    return PLAYER_STATS.skill_useable(skill_stats, cool_down_ticks);
  }
  inline void use() noexcept {
    cool_down_ticks = 0;
    PLAYER.flip = MOUSE_POS.x < CAMERA_X;
    PLAYER.sprite_counter = 0;
    PLAYER.action_state = attack_animation;
    PLAYER_STATS.use_skill(skill_stats);
  }
};

#include "skills/Dummy.h"
#include "skills/FireBall_Skill.h"
#include "skills/FireStrike_Skill.h"
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
