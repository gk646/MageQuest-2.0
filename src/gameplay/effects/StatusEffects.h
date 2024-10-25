#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STATUSEFFECTS_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STATUSEFFECTS_H_

struct Stun final : public StatusEffect {
  explicit Stun(int duration) : StatusEffect(true, 0, duration, EffectType::STUN) {}
  [[nodiscard]] Stun* Clone() const final { return new Stun(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.stunned = true;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { UpdateDuration(); }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.stunned = false;
  }
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
  }
};

struct Root final : public StatusEffect {
  float preValue = -1;
  explicit Root(int duration) : StatusEffect(true, 0, duration, EffectType::ROOT) {}
  [[nodiscard]] Root* Clone() const final { return new Root(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    preValue = stats.speed;
    stats.speed = 0;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { UpdateDuration(); }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.speed = preValue;
  }
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
  }
};
struct Berserk final : public StatusEffect {
  float attackBoost;
  Berserk(float percentValue, int duration)
      : StatusEffect(false, 0, duration, EffectType::BERSERK),
        attackBoost(percentValue) {}
  [[nodiscard]] Berserk* Clone() const final { return new Berserk(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[WEAPON_DAMAGE] *= attackBoost;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { UpdateDuration(); }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[WEAPON_DAMAGE] /= attackBoost;
  }
  //Uses always the longest and strongest value
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    if (((Berserk*)other)->attackBoost > attackBoost) {
      RemoveEffect(stats, self);
      attackBoost = ((Berserk*)other)->attackBoost;
      ApplyEffect(stats, self);
    }
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           attackBoost - 1);
  }
};
struct Slow final : public StatusEffect {
  float slowPercent;
  Slow(float slowPercent, int duration)
      : StatusEffect(true, 0, duration, EffectType::SLOW),
        slowPercent(slowPercent / 100) {}
  [[nodiscard]] Slow* Clone() const final { return new Slow(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] -= slowPercent;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { UpdateDuration(); }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] += slowPercent;
  }
  //Uses always the longest and strongest value
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    if (((Slow*)other)->slowPercent > slowPercent) {
      RemoveEffect(stats, self);
      slowPercent = ((Slow*)other)->slowPercent;
      ApplyEffect(stats, self);
    }
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           slowPercent * 100.0F);
  }
};
struct Burn final : public StatusEffect {
  DamageStats damageStats;
  Burn(float damage, int duration, int tickSpeed)
      : StatusEffect(true, tickSpeed, duration, EffectType::BURN),
        damageStats({DamageType::FIRE, damage}) {}
  [[nodiscard]] Burn* Clone() const final { return new Burn(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final {
    if (IsActiveTick()) {
      stats.TakeDamage(damageStats, self);
    }
    UpdateDuration();
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    damageStats.damage += ((Burn*)other)->damageStats.damage;
    stacks++;
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           damageStats.damage, (float)cadence / 60.0F);
  }
};
struct Poison final : public StatusEffect {
  DamageStats damageStats;
  Poison(float damage, int duration, int tick_speed)
      : StatusEffect(true, tick_speed, duration, EffectType::POISON),
        damageStats({DamageType::POISON, damage}) {}
  Poison(const Poison& p)
      : StatusEffect(p.isDebuff, p.cadence, p.duration, p.type),
        damageStats(p.damageStats) {}
  ~Poison() final = default;
  [[nodiscard]] Poison* Clone() const final { return new Poison(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final {
    if (IsActiveTick()) {
      stats.TakeDamage(damageStats, self);
    }
    UpdateDuration();
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    damageStats.damage += ((Poison*)other)->damageStats.damage;
    stacks++;
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           damageStats.damage, cadence / 60.0F);
  }
};
struct Swiftness final : public StatusEffect {
  float speedPercent;
  Swiftness(float value, int duration)
      : StatusEffect(false, 0, duration, EffectType::SWIFTNESS),
        speedPercent(value / 100) {}
  [[nodiscard]] Swiftness* Clone() const final { return new Swiftness(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] += speedPercent;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final {
    UpdateDuration();
    ;
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[SPEED_MULT_P] -= speedPercent;
  }
  //Uses always the longest and strongest value
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    if (((Swiftness*)other)->speedPercent > speedPercent) {
      RemoveEffect(stats, self);
      speedPercent = ((Swiftness*)other)->speedPercent;
      ApplyEffect(stats, self);
    }
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           speedPercent * 100.0F);
  }
};
struct SpellEchoCD final : public StatusEffect {
  inline static constexpr int SPELL_ECHO_CD = 60 * 15.0F;
  SpellEchoCD() : StatusEffect(true, 0, SPELL_ECHO_CD, EffectType::SPELL_ECHO_CD) {}
  [[nodiscard]] SpellEchoCD* Clone() const final { return new SpellEchoCD(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final { duration--; }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {}
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
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {}
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
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {}
};
struct Bleed final : public StatusEffect {
  DamageStats damageStats;
  Bleed(float damage, int duration, int tickSpeed)
      : StatusEffect(true, tickSpeed, duration, EffectType::BLEED),
        damageStats({DamageType::PHYSICAL, damage}) {}
  [[nodiscard]] Bleed* Clone() const final { return new Bleed(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final {
    if (IsActiveTick()) {
      stats.TakeDamage(damageStats, self);
    }
    UpdateDuration();
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    damageStats.damage += ((Bleed*)other)->damageStats.damage;
    stacks++;
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           damageStats.damage, (float)cadence / 60.0F);
  }
};
struct Resistance final : public StatusEffect {
  float resistanceAmount;
  Resistance(float percent, int duration)
      : StatusEffect(false, 0, duration, EffectType::RESISTANCE),
        resistanceAmount(percent / 100) {
    maxStacks = 15;
  }
  [[nodiscard]] Resistance* Clone() const final { return new Resistance(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[DAMAGE_RESISTANCE_P] += resistanceAmount;
  }
  void TickEffect(EntityStats& stats, const Entity* self) final { UpdateDuration(); }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {
    stats.effects[DAMAGE_RESISTANCE_P] -= resistanceAmount;
  }
  //Uses always the longest and strongest value
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    if (((Resistance*)other)->resistanceAmount > resistanceAmount) {
      RemoveEffect(stats, self);
      resistanceAmount = ((Resistance*)other)->resistanceAmount;
      ApplyEffect(stats, self);
    }
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           resistanceAmount * 100.0F);
  }
};
//In case of percent based val is a percent value from max health e.g. 0.01 for 1% each tick
struct Regeneration final : public StatusEffect {
  float val;
  float lastHealed = 0;
  bool isPercentFromMax;
  Regeneration(float val, int duration, int tickSpeed, bool isPercent)
      : StatusEffect(false, tickSpeed, duration, EffectType::REGENERATION),
        val(val),
        isPercentFromMax(isPercent) {}
  [[nodiscard]] Regeneration* Clone() const final { return new Regeneration(*this); }
  void ApplyEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void TickEffect(EntityStats& stats, const Entity* self) final {
    if (IsActiveTick()) {
      if (isPercentFromMax) {
        lastHealed = stats.GetMaxHealth() * val;
        stats.AddHealth(lastHealed);
      } else {
        stats.AddHealth(val);
      }
    }
    UpdateDuration();
  }
  void RemoveEffect(EntityStats& stats, const Entity* self) noexcept final {}
  void AddStack(EntityStats& stats, const StatusEffect* other,
                const Entity* self) noexcept final {
    TakeLongestDuration(other);
    MAX_STACKS_RETURN()
    val += ((Regeneration*)other)->val;
    stacks++;
  }
  [[nodiscard]] std::string GetToolTip() const noexcept final {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration,
                                           isPercentFromMax ? lastHealed : val,
                                           (float)cadence / 60.0F);
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STATUSEFFECTS_H_
