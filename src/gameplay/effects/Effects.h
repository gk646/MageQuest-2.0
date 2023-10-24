#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_EFFECTS_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_EFFECTS_H_
struct Stun final : public StatusEffect {
  explicit Stun(int duration)
      : StatusEffect(true, 0, duration, false, EffectType::STUN) {}
  [[nodiscard]] Stun* clone() const final { return new Stun(*this); }
  void ApplyEffect(EntityStats& stats) noexcept final { stats.stunned = true; }
  void TickEffect(EntityStats& stats) final { duration--; }
  void RemoveEffect(EntityStats& stats) noexcept final { stats.stunned = false; }
};
struct Root final : public StatusEffect {
  float preValue = -1;
  explicit Root(int duration)
      : StatusEffect(true, 0, duration, false, EffectType::ROOT) {}
  [[nodiscard]] Root* clone() const final { return new Root(*this); }
  void ApplyEffect(EntityStats& stats) noexcept final {
    preValue = stats.effects[SPEED_MULT_P];
    stats.effects[SPEED_MULT_P] = -1;
  }
  void TickEffect(EntityStats& stats) final {
    duration--;
    if (stats.effects[SPEED_MULT_P] != -1) {
      preValue = stats.effects[SPEED_MULT_P];
      stats.effects[SPEED_MULT_P] = -1;
    }
  }
  void RemoveEffect(EntityStats& stats) noexcept final {
    stats.effects[SPEED_MULT_P] = preValue;
  }
};
struct Slow final : public StatusEffect {
  float slow_percent;
  Slow(float value, int duration)
      : StatusEffect(true, 0, duration, false, EffectType::SLOW),
        slow_percent(value / 100) {}
  [[nodiscard]] Slow* clone() const final { return new Slow(*this); }
  void ApplyEffect(EntityStats& stats) noexcept final {
    stats.effects[SPEED_MULT_P] -= slow_percent;
  }
  void TickEffect(EntityStats& stats) final { duration--; }
  void RemoveEffect(EntityStats& stats) noexcept final {
    stats.effects[SPEED_MULT_P] += slow_percent;
  }
};
struct Burn final : public StatusEffect {
  DamageStats damage_stats;
  Burn(float damage, int duration, int tick_speed)
      : StatusEffect(true, tick_speed, duration, false, EffectType::BURN),
        damage_stats({DamageType::FIRE, damage}) {}
  [[nodiscard]] Burn* clone() const final { return new Burn(*this); }
  void ApplyEffect(EntityStats& stats) noexcept final {}
  void TickEffect(EntityStats& stats) final {
    if (is_damage_tick()) {
      stats.TakeDamage(damage_stats);
    }
    duration--;
  }
  void RemoveEffect(EntityStats& stats) noexcept final {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_EFFECTS_H_
