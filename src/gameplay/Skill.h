#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILL_H_

struct Skill {
  DamageStats damage_stats;
  SkillStats skill_stats;
  int cool_down_ticks = 0;
  bool from_player;
  int attack_animation = 0;
  Texture* icon = nullptr;
  Skill(SkillStats ability_stats, DamageStats damage_stats, bool from_player,
        int attack_animation, Texture* icon) noexcept
      : damage_stats(damage_stats),
        skill_stats(ability_stats),
        from_player(from_player),
        attack_animation(attack_animation),
        icon(icon) {
    cool_down_ticks = ability_stats.cool_down;
  }
  inline virtual void activate() = 0;
  inline void update() noexcept { cool_down_ticks++; };
  virtual void draw(float x, float y, float size) const noexcept {
    DrawTextureProFast(*icon, x , y , 0, WHITE);
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
        DrawLine(x + size / 2, y + size / 2, x + i, y, Colors::mediumLightGreyTransparent);
      }
      for (int i = side2; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + size, y + i, Colors::mediumLightGreyTransparent);
      }
      for (int i = side3; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x + size - i, y + size,
                 Colors::mediumLightGreyTransparent);
      }
      for (int i = side4; i <= size; i++) {
        DrawLine(x + size / 2, y + size / 2, x, y + size - i, Colors::mediumLightGreyTransparent);
      }
      for (int i = side5; i <= size / 2; i++) {
        DrawLine(x + size / 2, y + size / 2, x + i, y, Colors::mediumLightGreyTransparent);
      }
    }
  }
  static inline void DrawSupportBar(float x, float y, float percent) noexcept{
    DrawRectangleProFast(x-SCALE(2),y-SCALE(12),SCALE(53)*percent, SCALE(7),Colors::SUPPORT_BAR_ORANGE);
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
#include "skills/BlastHammer_Skill.h"
#include "skills/EnergySphere_Skill.h"
#include "skills/FireStrike_Skill.h"

inline static void Multiplayer::HandleProjectile(UDP_Projectile* data,const Entity* ptr) noexcept {
  switch (data->p_type) {
    case FIRE_BALL: {
      PROJECTILES.emplace_back(
          new FireBall({(float)data->x, (float)data->y}, !FRIENDLY_FIRE,
                       FireBall_Skill::LIFE_SPAN, FireBall_Skill::SPEED, data->damage,
                       HitType::ONE_HIT, {}, data->pov, {data->move_x, data->move_y},ptr));
      break;
    }
    case FIRE_STRIKE: {
      PROJECTILES.emplace_back(
          new FireBall({(float)data->x, (float)data->y}, !FRIENDLY_FIRE,
                       FireStrike_Skill::LIFE_SPAN, FireStrike_Skill::SPEED, data->damage,
                       HitType::CONTINUOUS, {}, data->pov, {data->move_x, data->move_y},ptr));
      break;
    }
    case BLAST_HAMMER: {
      PROJECTILES.emplace_back(new BlastHammer(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, BlastHammer_Skill::LIFE_SPAN,
          0, data->damage, HitType::ONE_TICK, {}, data->pov, {0, 0},
          RANGE_01(RNG_RANDOM) > 0.5F,ptr));
      break;
    }
    case ENERGY_SPHERE: {
      PROJECTILES.emplace_back(new EnergySphere(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, EnergySphere_Skill::LIFESPAN,
          EnergySphere_Skill::SPEED, data->damage, HitType::CONTINUOUS, {},
          {data->move_x, data->move_y},ptr));
      break;
    }
  }
}
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
