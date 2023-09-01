#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

#include "Stats.h"

struct StatusEffect {
  DamageStats damage_stats;

  StatusEffect(DamageStats damage_stats) : damage_stats(damage_stats) {}
  virtual void tick(EntityStats stats) = 0;
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
