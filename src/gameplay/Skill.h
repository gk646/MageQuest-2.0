#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILL_H_

struct Skill {
  DamageStats damage_stats;
  SkillStats ability_stats;
  float cool_down_ticks = 0;
  // Texture2D texture;
  Skill(SkillStats ability_stats, DamageStats damage_stats)
      : damage_stats(damage_stats), ability_stats(ability_stats) {
    cool_down_ticks = ability_stats.cool_down;
  }
  inline virtual void activate() = 0;
  inline virtual void update() = 0;
  void draw(float x, float y, float size) const noexcept {
    DrawRectangle(x, y, size, size, RED);
    int side1, side2, side3, side4, side5;
    float coolDownCoefficient;
    if (cool_down_ticks <= ability_stats.cool_down) {
      coolDownCoefficient = cool_down_ticks * (size*4 / ability_stats.cool_down);
      side1 = size/2;
      side2 = 0;
      side3 = 0;
      side4 = 0;
      side5 = 0;
    } else {
      coolDownCoefficient = 0;
      side1 = 100;
      side2 = 110;
      side3 = 110;
      side4 = 111;
      side5 = 110;
    }
    if (coolDownCoefficient > 0)
      side1 += static_cast<int>(coolDownCoefficient);
    if (coolDownCoefficient > size/2)
      side2 += static_cast<int>(coolDownCoefficient - size/2);
    if (coolDownCoefficient > size*1.5F)
      side3 += static_cast<int>(coolDownCoefficient - size*1.5F);
    if (coolDownCoefficient > size*2.5F)
      side4 += static_cast<int>(coolDownCoefficient - size*2.5F);
    if (coolDownCoefficient > size*3.5F)
      side5 += static_cast<int>(coolDownCoefficient - size*3.5F);

    for (int i = side1; i <= size; i++) {
      DrawLine(x + size/2, y + size/2, x + i, y, Colors::mediumVeryLight);
    }
    for (int i = side2; i <= size; i++) {
      DrawLine(x + size/2, y + size/2, x + size, y + i, Colors::mediumVeryLight);
    }
    for (int i = side3; i <= size; i++) {
      DrawLine(x + size/2, y + size/2, x + size - i, y + size, Colors::mediumVeryLight);
    }
    for (int i = side4; i <= size; i++) {
      DrawLine(x + size/2, y + size/2, x, y + size - i, Colors::mediumVeryLight);
    }
    for (int i = side5; i <= size/2; i++) {
      DrawLine(x + size/2, y + size/2, x + i, y, Colors::mediumVeryLight);
    }
  }
  inline bool use_able() const {
    return cool_down_ticks >= ability_stats.get_cd(PLAYER_STATS.combat_stats.cooldown_reduction) &&
           PLAYER_STATS.general.health >= ability_stats.health_cost &&
           PLAYER_STATS.general.mana >= ability_stats.mana_cost;
  }
  inline void use() noexcept {
    cool_down_ticks = 0;
    //PLAYER_STATS.general.mana -= ability_stats.mana_cost;
  }
};

#include "abilities/FireStrike.h"
#include "abilities/FireBall.h"
#include "abilities/Dummy.h"
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
