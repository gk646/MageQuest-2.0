#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

struct MonsterScaler {
  float baseHealth;
  float healthPerLevel;
  float speed;
  float damage;
  int16_t attackCD;
  uint8_t attackRange;
  uint8_t chaseRange;
  [[nodiscard]] inline float GetMaxHealth(uint8_t level) const noexcept {
    float maxHealth =
        baseHealth + ((level - 1) * healthPerLevel) * cxstructs::fast_sqrt(level);
    maxHealth *= cxstructs::fast_sqrt(level);
    maxHealth *= DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY];
    return maxHealth;
  }
};

struct SkillStats {
  float manaCost = 0;
  float healthCost = 0;
  float baseDamage = 0;
  float specialVal1 = 0;
  float specialVal2 = 0;
  uint16_t castTime = 0;
  uint16_t coolDownTicks = 0;
  uint16_t range = 0;
  ProjectileType type = ProjectileType::LOCKED;
};

struct DamageStats {
  float damage = 1;
  float critChance;
  float critDamage;
  DamageType dmgType = DamageType::FIRE;
  DamageStats(DamageType type, float damage, float critChance = 5, float critDamage = 25)
      : damage(damage), dmgType(type), critDamage(critDamage), critChance(critChance) {}
  inline bool operator==(const DamageStats& d) const noexcept {
    return dmgType == d.dmgType && dmgType == d.dmgType && damage == d.damage;
  }
};

struct PlayerSpentSkillPoints {
  int16_t spentAttributePoints[9] = {0};
  int16_t attributePointsToSpend = 0;
  int16_t spentTalentPoints = 0;
  int16_t talentPointsToSpend = 25;
  inline bool SpendAttributePoint(uint8_t i) noexcept {
    if (attributePointsToSpend > 0) {
      spentAttributePoints[i]++;
      attributePointsToSpend--;
      return true;
    }
    return false;
  }
  inline bool SpendTalentPoint() noexcept {
    if (talentPointsToSpend > 0) {
      spentTalentPoints++;
      talentPointsToSpend--;
      return true;
    }
    return false;
  }
  inline void AddLevelUPPoints() noexcept {
    attributePointsToSpend += 3;
    talentPointsToSpend++;
  }
  [[nodiscard]] inline bool IsDefaultValue(Stat stat) const noexcept;
  [[nodiscard]] inline bool HasAttributePointsToSpend() const noexcept {
    return attributePointsToSpend > 0;
  }
  [[nodiscard]] inline bool HasTalentPointsToSpend() const noexcept {
    return talentPointsToSpend > 0;
  }
};

inline static PlayerSpentSkillPoints PLAYER_SPENT_POINTS{};

struct EntityStats {
  float effects[STATS_ENDING] = {0};
  float health = 10;
  float mana = 20;
  float speed = 2.6F;
  float shield = 0;
  uint8_t level = 1;
  bool stunned = false;
  DamageType lastHitType = DamageType::TRUE_DMG;
  EntityStats() {
    effects[CRIT_CHANCE] = 5;
    effects[CRIT_DAMAGE_P] = 0.5F;
    effects[HEALTH_REGEN] = 0.2F;
    effects[MANA_REGEN] = 1;
    effects[MAX_HEALTH] = 10;
    effects[MAX_MANA] = 20;
    effects[WEAPON_DAMAGE] = 0;
  };
  EntityStats(const MonsterScaler& scaler, uint8_t level) noexcept
      : level(level), speed(scaler.speed) {
    effects[MAX_HEALTH] = scaler.GetMaxHealth(level);
    health = effects[MAX_HEALTH];
    effects[CRIT_CHANCE] = 5;
    effects[CRIT_DAMAGE_P] = 0.5F;
    effects[HEALTH_REGEN] = effects[MAX_HEALTH] / 7200;
  }
  inline void Update() noexcept {
    float max_mana_value = GetMaxMana();
    if (mana < max_mana_value) {
      mana = std::min(mana + GetManaRegen() / 60, max_mana_value);
    } else {
      mana = max_mana_value;
    }

    float max_health_value = GetMaxHealth();
    if (health < max_health_value) {
      health = std::min(health + GetHealthRegen() / 60, max_health_value);
    } else {
      health = max_health_value;
    }

    float max_shield = effects[MAX_SHIELD];
    if (shield < max_shield && mana > 0) {
      shield += effects[MANA_REGEN] / 60;
      mana -= effects[MANA_REGEN] / 60;
    } else if (shield > max_shield) {
      shield = std::max(shield - effects[MANA_REGEN] / 60, max_shield);
    }
  }
  [[nodiscard]] inline bool IsSkillUsable(const SkillStats& stats,
                                          float ticks_done) const noexcept {
    return !stunned && ticks_done >= (float)stats.coolDownTicks * (1 - effects[CDR_P]) &&
           mana >= stats.manaCost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline int GetRemainingCD(const SkillStats& stats) noexcept {
    return stats.coolDownTicks * (1 - effects[CDR_P]);
  }
  inline void ApplySkillCosts(const SkillStats& stats) noexcept {
    mana -= stats.manaCost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline void EquipItem(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] += effect_arr[i];
    }
    ReCalculatePlayerStats();
  }
  inline void UnEquipItem(const float* effect_arr) noexcept;
  inline float GetAbilityDmg(const DamageStats& stats) noexcept {
    switch (stats.dmgType) {
      case DamageType::FIRE:
        return (stats.damage + effects[WEAPON_DAMAGE]) * (1 + effects[FIRE_DMG_P]);
      case DamageType::POISON:
        return (stats.damage + effects[WEAPON_DAMAGE]) * (1 + effects[POISON_DMG_P]);
      case DamageType::ICE:
        return (stats.damage + effects[WEAPON_DAMAGE]) * (1 + effects[ICE_DMG_P]);
      case DamageType::ARCANE:
        return (stats.damage + effects[WEAPON_DAMAGE]) * (1 + effects[ARCANE_DMG_P]);
      case DamageType::DARK:
        return (stats.damage + effects[WEAPON_DAMAGE]) * (1 + effects[DARK_DMG_P]);
      case DamageType::PHYSICAL:
        return stats.damage + effects[WEAPON_DAMAGE];
      case DamageType::TRUE_DMG:
        return stats.damage + effects[WEAPON_DAMAGE];
    }
  }
  inline float TakeDamage(const DamageStats& stats) {
    float armour = GetArmour();
    float total_damage = RollCriticalHit(stats);

    if (stats.dmgType == DamageType::PHYSICAL) {
      total_damage *= 1 - armour / (level * 50.0F);
    } else if (stats.dmgType != DamageType::TRUE_DMG) {
      if (shield >= total_damage) {
        shield -= total_damage;
        return total_damage;
      } else {
        total_damage -= shield;
        shield = 0;
      }
    }
    if (health > 0) {
      lastHitType = stats.dmgType;
    }
    health -= total_damage;
    return total_damage;
  }
  inline void RefillStats() noexcept {
    mana = GetMaxMana();
    health = GetMaxHealth();
    shield = effects[MAX_SHIELD];
  }
  inline void SpendAttributePoint(uint8_t i) noexcept {
    if (PLAYER_SPENT_POINTS.SpendAttributePoint(i)) {
      effects[i]++;
      ReCalculatePlayerStats();
    }
  }
  [[nodiscard]] inline float GetMaxHealth() const noexcept {
    return effects[MAX_HEALTH] * (1 + effects[HEALTH_MULT_P]);
  }
  [[nodiscard]] inline float GetMaxMana() const noexcept {
    return effects[MAX_MANA] * (1 + effects[MANA_MULT_P]);
  }
  [[nodiscard]] inline float GetSpeed() const noexcept {
    return speed * (1 + effects[SPEED_MULT_P]);
  }
  [[nodiscard]] inline float GetManaRegen() const noexcept {
    return effects[MANA_REGEN] * (1 + effects[MANA_REGEN_MULT_P]);
  }
  [[nodiscard]] inline float GetHealthRegen() const noexcept {
    return effects[HEALTH_REGEN] * (1 + effects[HEALTH_REGEN_MULT_P]);
  }
  [[nodiscard]] inline float GetArmour() const noexcept {
    return effects[ARMOUR] * (1 + effects[ARMOUR_MULT_P]);
  }
  [[nodiscard]] inline float GetBagSlots() const noexcept { return effects[BAG_SLOTS]; }
  inline void ReCalculatePlayerStats() noexcept {
    RemoveEffects();
    effects[MAX_HEALTH] = 20.0F + effects[VITALITY] * 5.0F;
    effects[MAX_MANA] = 10.0F + effects[INTELLIGENCE] * 7.5F;

    effects[HEALTH_REGEN] = 0.2F + effects[ENDURANCE] / 18.0F;
    effects[MANA_REGEN] = 1 + effects[WISDOM] / 9.0F;

    effects[SPEED_MULT_P] = (effects[AGILITY] / 100) * std::sqrt(level);
    ApplyEffects();
  }
  [[nodiscard]] static inline float RollCriticalHit(const DamageStats& stats) noexcept {
    if (RANGE_100_FLOAT(RNG_ENGINE) < stats.critChance) {
      return stats.damage * (1 + stats.critDamage);
    }
    return stats.damage;
  }

 private:
  static inline void RemoveEffects() noexcept;
  static inline void ApplyEffects() noexcept;
};
inline EntityStats PLAYER_STATS;

inline static std::unordered_map<MonsterType, MonsterScaler> monsterIdToScaler{};
bool PlayerSpentSkillPoints::IsDefaultValue(Stat stat) const noexcept {
  return spentAttributePoints[stat] == (int)PLAYER_STATS.effects[stat];
}
#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
