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
    DrawRectangleLinesEx({x, y, size, size}, 3, GRAY);
    //DrawTexture(texture, x, y, WHITE);
  }
  inline bool use_able() const {
    return cool_down_ticks >=
               ability_stats.cool_down -
                   (ability_stats.cool_down * PLAYER_STATS.combat_stats.cooldown_reduction / 100) &&
           PLAYER_STATS.general.health >= ability_stats.health_cost &&
           PLAYER_STATS.general.mana >= ability_stats.mana_cost;
  }
  inline void use() {
    cool_down_ticks = 0;
    //PLAYER_STATS.general.mana -= ability_stats.mana_cost;
  }
};

#include "abilities/FireStrike.h"
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
