#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_

struct Burn final : StatusEffect {
  DamageStats damage_stats{};
  Burn(float damage, int duration, int tick_speed)
      : StatusEffect(StatusEffectType::DE_BUFF,  tick_speed, duration, false),
        damage_stats({DamageType::FIRE, damage}) {}
  StatusEffect* clone() const override { return new Burn(*this); }
  void activate(EntityStats& stats) const noexcept final {}
  void tick(EntityStats& stats) noexcept final  {
    if (is_damage_tick()) {
      stats.take_damage(damage_stats);
    }
    duration--;
  }
  void deactivate(EntityStats& stats) const noexcept final {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_BURN_H_
