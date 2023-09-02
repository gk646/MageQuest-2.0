#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_

#include "../StatusEffect.h"

struct Slow : StatusEffect {
  float slow_percent;
  Slow(float value, int duration)
      : StatusEffect(StatusEffectType::DE_BUFF, SourceType::MAGICAL, 0, duration, false),
        slow_percent(value / 100) {}
  virtual StatusEffect* clone() const override { return new Slow(*this); }
  void activate(EntityStats& stats) const final { stats.general.speed *= slow_percent; }
  void tick(EntityStats& stats) final { duration--; }
  void deactivate(EntityStats& stats) const final {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_
