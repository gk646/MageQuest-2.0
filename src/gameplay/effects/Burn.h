#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
#include "../StatusEffect.h"

struct Burn final: StatusEffect {
  DamageStats damage_stats;
  Burn(DamageStats damage_stats, int duration, int tick_speed)
      : StatusEffect(StatusEffectType::DE_BUFF, SourceType::MAGICAL, tick_speed, duration, false),
        damage_stats(damage_stats) {}
  StatusEffect* clone() const override { return new Burn(*this); }
  void activate(EntityStats& stats) const final {}
  void tick(EntityStats& stats) final {
    if (is_damage_tick()) {
      stats.general.take_damage(damage_stats);
    }
    duration--;
  }
  void deactivate(EntityStats& stats) const final {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
