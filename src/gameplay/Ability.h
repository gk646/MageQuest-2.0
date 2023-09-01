#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITY_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITY_H_

#include "../system/GlobalVariables.h"
#include "Stats.h"

struct Ability {
  DamageStats damage_stats;
  AbilityStats ability_stats;
  float cool_down_ticks = 0;

  Ability(AbilityStats ability_stats, DamageStats damage_stats)
      : damage_stats(damage_stats), ability_stats(ability_stats) {}
  virtual void activate() = 0;
  virtual void update() = 0;
  [[nodiscard]] bool use_able(EntityStats entity_stats) const {
    return cool_down_ticks >=
               ability_stats.cool_down -
                   (entity_stats.combat_stats.cooldown_reduction * ability_stats.cool_down) &&
           ability_stats.health_cost >= entity_stats.general.health &&
           ability_stats.mana_cost >= entity_stats.general.mana;
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITY_H_
