#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_

struct Slow final : StatusEffect {
  float slow_percent;
  Slow(float value, int duration)
      : StatusEffect(StatusEffectType::DE_BUFF, 0, duration, false),
        slow_percent(value / 100) {}
  virtual StatusEffect* clone() const override { return new Slow(*this); }
  void activate(EntityStats& stats) const noexcept final { stats.effects[SPEED_MULT_P]-=0.5; }
  void tick(EntityStats& stats) noexcept final { duration--; }
  void deactivate(EntityStats& stats) const noexcept final {
    stats.effects[SPEED_MULT_P]+=0.5;
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_SLOW_H_
