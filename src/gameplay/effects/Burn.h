#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_

struct Burn final : public StatusEffect {
  DamageStats damage_stats;
  Burn(float damage, int duration, int tick_speed)
      : StatusEffect(true,  tick_speed, duration, false, EffectType::BURN),
        damage_stats({DamageType::FIRE, damage}) {}
  [[nodiscard]] Burn* clone() const final { return new Burn(*this); }
  void ApplyEffect(EntityStats& stats) const noexcept final {}
  void TickEffect(EntityStats& stats) final  {
    if (is_damage_tick()) {
      stats.take_damage(damage_stats);
    }
    duration--;
  }
  void RemoveEffect(EntityStats& stats) const noexcept final {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
