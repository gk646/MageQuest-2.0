#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

struct MonsterScaler {
  float baseHealth = 5;
  float healthPerLevel = 1;
  float speed = 2;
  int16_t attackCD = 50;
  uint8_t attackRange = 5;
  uint8_t chaseRange = 8;
  [[nodiscard]] inline float GetMaxHealth(uint8_t level) const noexcept {
    return (baseHealth +
            ((level - 1.0F) * healthPerLevel * cxstructs::fast_sqrt(level))) *
           DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY];
  }
};

struct MonsterAttackStats {
  int16_t attackCooldown;
  int16_t currentCooldown = 0;
  uint8_t attackRange;
  uint8_t chaseRange;
  explicit MonsterAttackStats(const MonsterScaler& scaler)
      : attackCooldown(scaler.attackCD),
        attackRange(scaler.attackRange),
        chaseRange(scaler.chaseRange) {}
  [[nodiscard]] inline bool IsAttackReady(int8_t actionState) const noexcept {
    return actionState == 0 && currentCooldown <= 0;
  }
  inline void Update(int8_t actionState) noexcept { currentCooldown -= actionState == 0; }
  inline void ResetCooldown() noexcept { currentCooldown = attackCooldown; }
};


struct SkillStats {
  float manaCost = 0;
  float healthCost = 0;
  float baseDamage = 0;
  float speed = 0;
  float specialVal1 = 0;
  float specialVal2 = 0;
  uint16_t castTime = 0;
  uint16_t coolDownTicks = 0;
  uint16_t range = 0;
  uint16_t lifeSpan = 0;
};

struct DamageStats {
  float damage = 1;
  DamageType dmgType = DamageType::FIRE;
  DamageStats(DamageType type, float damage) : damage(damage), dmgType(type) {}
  inline bool operator==(const DamageStats& d) const noexcept {
    return dmgType == d.dmgType && dmgType == d.dmgType && damage == d.damage;
  }
};

struct SpentAttributePoints {
  float spentPoints[9] = {0};
  uint16_t pointsToSpend = 0;
  inline bool SpendPoint(uint8_t i) noexcept {
    if (pointsToSpend >= 1) {
      spentPoints[i]++;
      pointsToSpend--;
      return true;
    }
    return false;
  }
  inline void LevelUP() noexcept { pointsToSpend += 3; }
  [[nodiscard]] inline bool IsDefaultValue(Stat stat) const noexcept;
  [[nodiscard]] inline bool HasPointsToSpend() const noexcept {
    return pointsToSpend >= 1;
  }
};
inline static SpentAttributePoints PLAYER_SPENT_POINTS{};

struct EntityStats {
  float effects[STATS_ENDING] = {0};
  float health = 10;
  float mana = 20;
  float speed = 2.6F;
  float shield = 0;
  uint8_t level = 1;
  bool stunned = false;
  EntityStats() noexcept { InitPlayer(); };
  EntityStats(const MonsterScaler& scaler, uint8_t level) noexcept
      : level(level), speed(scaler.speed) {
    effects[MAX_HEALTH] = scaler.GetMaxHealth(level);
    effects[MAX_HEALTH] *= health = effects[MAX_HEALTH];
  }
  inline void update() noexcept {
    float max_mana_value = GetMaxMana();
    if (mana < max_mana_value) {
      mana = std::min(mana + effects[MANA_REGEN] / 60, max_mana_value);
    } else {
      mana = max_mana_value;
    }

    float max_health_value = GetMaxHealth();
    if (health < max_health_value) {
      health = std::min(health + effects[HEALTH_REGEN] / 60, max_health_value);
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
  [[nodiscard]] inline bool skill_useable(const SkillStats& stats,
                                          int ticks_done) const noexcept {
    return !stunned && ticks_done >= stats.coolDownTicks * (1 - effects[CDR_P]) &&
           mana >= stats.manaCost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline int GetRemainingCD(const SkillStats& stats) noexcept {
    return stats.coolDownTicks * (1 - effects[CDR_P]);
  }
  inline void UseSkill(const SkillStats& stats) noexcept {
    mana -= stats.manaCost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline void EquipItem(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] += effect_arr[i];
    }
    ReCalculatePlayerStats();
  }
  inline void UnEquipItem(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] -= effect_arr[i];
    }
    ReCalculatePlayerStats();
  }
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
    float& armour = effects[ARMOUR];
    float& armour_mult = effects[ARMOUR_MULT_P];

    float total_damage = RollCriticalHit(stats.damage);

    if (stats.dmgType == DamageType::PHYSICAL) {
      total_damage *= 1 - (armour * (1 + armour_mult)) / (level * 50.0F);
    } else if (stats.dmgType != DamageType::TRUE_DMG) {
      if (shield >= total_damage) {
        shield -= total_damage;
        return total_damage;
      } else {
        total_damage -= shield;
        shield = 0;
      }
    }
    health -= total_damage;
    return total_damage;
  }
  inline void RefillStats() noexcept {
    mana = GetMaxMana();
    health = GetMaxHealth();
    shield = effects[MAX_SHIELD];
  }
  static inline void RemoveEffects() noexcept;
  static inline void ApplyEffects() noexcept;
  inline void ReCalculatePlayerStats() noexcept {
    RemoveEffects();
    effects[MAX_HEALTH] = 20.0F + effects[VITALITY] * 5.0F;
    effects[MAX_MANA] = 10.0F + effects[INTELLIGENCE] * 7.5F;

    effects[HEALTH_REGEN] = 0.2F + effects[ENDURANCE] / 18.0F;
    effects[MANA_REGEN] = 1 + effects[WISDOM] / 9.0F;

    effects[SPEED_MULT_P] = (effects[AGILITY] / 100) * std::sqrt(level);
    ApplyEffects();
  }
  inline void SpendAttributePoint(uint8_t i) noexcept {

    if (PLAYER_SPENT_POINTS.SpendPoint(i)) {
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

 private:
  [[nodiscard]] inline float RollCriticalHit(float damage) const noexcept {
    if (RANGE_100_FLOAT(RNG_ENGINE) < effects[CRIT_CHANCE]) {
      return damage * (1 + effects[CRIT_DAMAGE_P]);
    }
    return damage;
  }
  inline void InitPlayer() noexcept {
    effects[CRIT_CHANCE] = 5;
    effects[CRIT_DAMAGE_P] = 0.5F;
    effects[HEALTH_REGEN] = 0.2F;
    effects[MANA_REGEN] = 1;
    effects[MAX_HEALTH] = 10;
    effects[MAX_MANA] = 20;
    effects[WEAPON_DAMAGE] = 0;
  }
};
inline EntityStats PLAYER_STATS;
inline static std::unordered_map<MonsterType, MonsterScaler> monsterIdToScaler{};
bool SpentAttributePoints::IsDefaultValue(Stat stat) const noexcept {
  return spentPoints[stat] - PLAYER_STATS.effects[stat] < 0.05F;
}
#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
