#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
#include "../StatusEffect.h"

struct Burn : StatusEffect {

  Burn(float value, float cadence, int duration)
      : StatusEffect(EffectStats{StatusEffectType::DE_BUFF, SourceType::MAGICAL, 0, duration},
                     DamageStats{SourceType::MAGICAL, MagicType::ICE, value}) {}

  void tick(EntityStats& stats) final {
    if (is_damage_tick()) {
      stats.get_damage(damage_stats);
    }
    current_ticks++;
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
