#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_

struct Slow final :public StatusEffect {
  float slow_percent;
  Slow(float value, int duration)
      : StatusEffect(true, 0, duration, false, EffectType::SLOW),
        slow_percent(value / 100) {}
   [[nodiscard]] Slow* clone() const final { return new Slow(*this); }
  void ApplyEffect(EntityStats& stats) const noexcept final {
    stats.effects[SPEED_MULT_P]-=slow_percent;
  }
  void TickEffect(EntityStats& stats) final {
    duration--;
  }
  void RemoveEffect(EntityStats& stats) const noexcept final {
    stats.effects[SPEED_MULT_P]+=slow_percent;
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_
