#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

struct SkillStats {
  float cool_down = 0;
  float mana_cost = 0;
  float health_cost = 0;
};

struct DamageStats {
  DamageType damage_type = DamageType::FIRE;
  float damage = 1;
  inline bool operator==(const DamageStats& d) const noexcept {
    return damage_type == d.damage_type && damage_type == d.damage_type &&
           damage == d.damage;
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
  float speed = 2.5;
  uint8_t level = 1;
  float shield = 0;
  bool stunned = false;
  EntityStats() {
    Init();
    effects[HEALTH_REGEN] = 0.2F;
    effects[MANA_REGEN] = 1;
    effects[MAX_HEALTH] = 10;
    effects[MAX_MANA] = 20;
    effects[WEAPON_DAMAGE] = 10;
  };
  EntityStats(float base_health, int level, float per_level, float speed) noexcept
      : level(level), speed(speed) {
    Init();
    effects[MAX_HEALTH] = base_health + ((level - 1) * per_level * std::sqrt(level));
    effects[MAX_HEALTH] *= DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY];
    health = effects[MAX_HEALTH];
  }
  inline void update() noexcept {
    float max_mana_value = get_max_mana();
    if (mana < max_mana_value) {
      mana = std::min(mana + effects[MANA_REGEN] / 60, max_mana_value);
    } else {
      mana = max_mana_value;
    }

    float max_health_value = get_max_health();
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
    return !stunned && ticks_done >= stats.cool_down * (1 - effects[CDR_P]) &&
           mana >= stats.mana_cost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline int GetRemainingCD(const SkillStats& stats) noexcept {
    return stats.cool_down * (1 - effects[CDR_P]);
  }
  inline void use_skill(const SkillStats& stats) noexcept {
    mana -= stats.mana_cost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline void equip_item(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] += effect_arr[i];
    }
    ReCalculatePlayerStats();
  }
  inline void un_equip_item(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] -= effect_arr[i];
    }
    ReCalculatePlayerStats();
  }
  inline float get_ability_dmg(DamageStats stats) noexcept {
    switch (stats.damage_type) {
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
  inline float take_damage(const DamageStats& stats) {
    float& armour = effects[ARMOUR];
    float& armour_mult = effects[ARMOUR_MULT_P];

    float total_damage = RollCriticalHit(stats.damage);

    if (stats.damage_type == DamageType::PHYSICAL) {
      total_damage *= (armour * (1 + armour_mult)) / (level * 50.0F);
    } else if (stats.damage_type != DamageType::TRUE_DMG) {
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
  inline void refill_stats() noexcept {
    mana = get_max_mana();
    health = get_max_health();
    shield = effects[MAX_SHIELD];
  }
  [[nodiscard]] inline float get_max_health() const noexcept {
    return effects[MAX_HEALTH] * (1 + effects[HEALTH_MULT_P]);
  }
  [[nodiscard]] inline float get_max_mana() const noexcept {
    return effects[MAX_MANA] * (1 + effects[MANA_MULT_P]);
  }
  [[nodiscard]] inline float get_speed() const noexcept {
    return speed * (1 + effects[SPEED_MULT_P]);
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
 private:
  inline float RollCriticalHit(float damage) const noexcept{
    if(RANGE_100_FLOAT(RNG_ENGINE) < effects[CRIT_CHANCE]){
      return damage* (1 + effects[CRIT_DAMAGE_P]);
    }
  }
  inline void Init()noexcept{
    effects[CRIT_CHANCE] = 5;
    effects[CRIT_DAMAGE_P] = 0.5F;
  }
};
inline EntityStats PLAYER_STATS;

bool SpentAttributePoints::IsDefaultValue(Stat stat) const noexcept {
  return spentPoints[stat] == PLAYER_STATS.effects[stat];
}
#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
