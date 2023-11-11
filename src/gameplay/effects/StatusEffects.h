#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STATUSEFFECTS_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STATUSEFFECTS_H_
struct Stun final : public StatusEffect {
  explicit Stun(int duration) : StatusEffect(true, 0, duration, EffectType::STUN) {}
  [[nodiscard]] Stun* Clone() const final { return new Stun(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.stunned = true;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { duration--; }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.stunned = false;
  }
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {
    duration = other->duration;
  }
};
struct Root final : public StatusEffect {
  float preValue = -1;
  explicit Root(int duration) : StatusEffect(true, 0, duration, EffectType::ROOT) {}
  [[nodiscard]] Root* Clone() const final { return new Root(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    preValue = stats.effects[SPEED_MULT_P];
    stats.effects[SPEED_MULT_P] = -1;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final {
    duration--;
    if (stats.effects[SPEED_MULT_P] != -1) {
      preValue = stats.effects[SPEED_MULT_P];
      stats.effects[SPEED_MULT_P] = -1;
    }
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] = preValue;
  }
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {}
};
struct Slow final : public StatusEffect {
  float slow_percent;
  Slow(float value, int duration)
      : StatusEffect(true, 0, duration, EffectType::SLOW), slow_percent(value / 100) {}
  [[nodiscard]] Slow* Clone() const final { return new Slow(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] -= slow_percent;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { duration--; }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] += slow_percent;
  }
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {}
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           slow_percent * 100.0F);
  }
};
struct Burn final : public StatusEffect {
  DamageStats damage_stats;
  Burn(float damage, int duration, int tick_speed)
      : StatusEffect(true, tick_speed, duration, EffectType::BURN),
        damage_stats({DamageType::FIRE, damage}) {}
  [[nodiscard]] Burn* Clone() const final { return new Burn(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final {
    if (IsDamageTick()) {
      stats.TakeDamage(damage_stats, self);
    }
    duration--;
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {}
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           damage_stats.damage, cadence);
  }
};
struct SpellEchoCD final : public StatusEffect {
  inline static constexpr int SPELL_ECHO_CD = 60 * 15.0F;
  SpellEchoCD() : StatusEffect(true, 0, SPELL_ECHO_CD, EffectType::SPELL_ECHO_CD) {}
  [[nodiscard]] SpellEchoCD* Clone() const final { return new SpellEchoCD(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final { duration--; }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {}
};

struct ElementalEquilibriumBuff final : public StatusEffect {
  inline static constexpr int ELEMENTAL_EQUILIBRIUM_BUFF = 60 * 10;
  DamageType excludedType;
  explicit ElementalEquilibriumBuff(DamageType excludedType)
      : StatusEffect(false, 0, ELEMENTAL_EQUILIBRIUM_BUFF,
                     EffectType::ELEMENTAL_EQUILIBRIUM_BUFF),
        excludedType(excludedType) {}
  [[nodiscard]] ElementalEquilibriumBuff* Clone() const final {
    return new ElementalEquilibriumBuff(*this);
  }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final { duration--; }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {}
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, FLT_MAX,
                                           FLT_MAX, FLT_MAX,
                                           damageTypeNames[excludedType].c_str());
  }
};
struct ElementalEquilibriumCD final : public StatusEffect {
  inline static constexpr int ELEMENTAL_EQUILIBRIUM_CD = 60 * 20;
  ElementalEquilibriumCD()
      : StatusEffect(true, 0, ELEMENTAL_EQUILIBRIUM_CD,
                     EffectType::ELEMENTAL_EQUILIBRIUM_CD) {}
  [[nodiscard]] ElementalEquilibriumCD* Clone() const final {
    return new ElementalEquilibriumCD(*this);
  }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final { duration--; }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {}
};
struct Poison final : public StatusEffect {
  DamageStats damage_stats;
  Poison(float damage, int duration, int tick_speed)
      : StatusEffect(true, tick_speed, duration, EffectType::POISON),
        damage_stats({DamageType::POISON, damage}) {}
  Poison(const Poison& p)
      : StatusEffect(p.isDebuff, p.cadence, p.duration, p.type),
        damage_stats(p.damage_stats) {}
  ~Poison() final= default;
  [[nodiscard]] Poison* Clone() const final { return new Poison(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final {
    if (IsDamageTick()) {
      stats.TakeDamage(damage_stats, self);
    }
    duration--;
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(const StatusEffect* other, const Entity* self) noexcept final {
    duration = other->duration;
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           damage_stats.damage, cadence);
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STATUSEFFECTS_H_
