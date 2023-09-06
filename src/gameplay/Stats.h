#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

struct EffectStats {
  float effects[50] = {0};
};

struct SkillStats {
  float cool_down = 0;
  float mana_cost = 0;
  float health_cost = 0;

  inline float get_cd(float cd_percent) const noexcept {}
};

struct DamageStats {
  DamageType damage_type = DamageType::FIRE;
  float damage = 1;
  inline bool operator==(const DamageStats& d) const noexcept {
    return damage_type == d.damage_type && damage_type == d.damage_type &&
           damage == d.damage;
  }
};
/*
struct ArmourStats {
  float physical_armour = 0;
  float magical_armour = 0;

  float base_physical_armour = 0;
  float base_magical_armour = 0;

  inline void reset_to_base() noexcept {
    physical_armour = base_physical_armour;
    magical_armour = base_magical_armour;
  }
};

struct CombatStats {
  //Percent Values
  int16_t cooldown_reduction = 0;
  int16_t health_cost_reduction = 0;
  int16_t mana_cost_reduction = 0;

  bool stunned = false;
};

struct Abilities {
  int8_t strength = 0;
  int8_t wisdom = 0;
  int8_t dexterity = 0;
  int8_t intelligence = 0;
  int8_t charisma = 0;
  int8_t constitution = 0;

  int8_t base_strength = 0;
  int8_t base_wisdom = 0;
  int8_t base_dexterity = 0;
  int8_t base_intelligence = 0;
  int8_t base_charisma = 0;
  int8_t base_constitution = 0;

  inline void reset_to_base() noexcept {
    strength = base_strength;
    wisdom = base_wisdom;
    dexterity = base_dexterity;
    intelligence = base_intelligence;
    charisma = base_charisma;
    constitution = base_constitution;
  }
};

struct GeneralStats {
  // ArmourStats armour_stats;
  float mana = 20;
  float concentration = 10;
  float health = 10;

  float speed = 3;  //pixel per tick
  float max_health = 10;
  float max_mana = 20;
  float max_concentration = 10;
  float health_regen = 0.2;  //per second
  float mana_regen = 1;      //per second

  float base_speed = 3;
  float base_max_health = 10;
  float base_max_mana = 20;
  float base_max_concentration = 10;
  float base_health_regen = 0.2;  //per second
  float base_mana_regen = 1;      //per second
  inline void reset_to_base() noexcept {
    //armour_stats.reset_to_base();

    speed = base_speed;
    max_health = base_max_health;
    max_mana = base_max_mana;
    max_concentration = base_max_concentration;
  }
  inline void update() {}
  inline void apply_monster_scaling(int level) {}
};
*/

struct EntityStats {
  float effects[STATS_ENDING] = {0};
  float health = 10;
  float mana = 20;
  float speed = 3;
  int level = 1;
  bool stunned = false;
  EntityStats() {
    effects[HEALTH_REGEN] = 0.2F;
    effects[MANA_REGEN] = 1;
    effects[MAX_HEALTH] = 10;
    effects[MAX_MANA] = 20;
  };
  EntityStats(float base_health, int level) noexcept : level(level) {
    effects[MAX_HEALTH] = base_health;
    effects[MAX_HEALTH] *= std::pow(1.2F, level);
    effects[MAX_HEALTH] *= DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY];
    health = effects[MAX_HEALTH];
  }
  inline void update() noexcept {
    float max_mana_value = get_max_mana();
    if (mana < max_mana_value) {
      mana = std::min(mana + effects[MANA_REGEN] / 60, max_mana_value);
    } else {
      mana = std::max(mana - effects[MANA_REGEN] / 60, max_mana_value);
    }

    float max_health_value = get_max_health();
    if (health < max_health_value) {
      health = std::min(health + effects[HEALTH_REGEN] / 60, max_health_value);
    } else {
      health = std::max(health - effects[HEALTH_REGEN] / 60, max_health_value);
    }
  }
  inline bool skill_useable(const SkillStats& stats,
                            float ticks_done) const noexcept {
    return !stunned && ticks_done >= stats.cool_down * (1 - effects[CDR_P]) &&
           mana >= stats.mana_cost * (1 + effects[MANA_COST_REDUCTION_P]);
  }
  inline void use_skill(const SkillStats& stats) noexcept {
    mana -= stats.mana_cost * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline void equip_item(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] += effect_arr[i];
    }
  }
  inline void un_equip_item(const float* effect_arr) noexcept {
    for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
      effects[i] -= effect_arr[i];
    }
  }
  inline float get_weapon_damage(DamageType type) noexcept {
    switch (type) {
      case DamageType::FIRE:
        return effects[WEAPON_DAMAGE] * effects[FIRE_DMG];
      case DamageType::POISON:
        return effects[WEAPON_DAMAGE] * effects[POISON_DMG];
      case DamageType::ICE:
        return effects[WEAPON_DAMAGE] * effects[ICE_DMG];
      case DamageType::ARCANE:
        return effects[WEAPON_DAMAGE] * effects[ARCANE_DMG];
      case DamageType::DARK:
        return effects[WEAPON_DAMAGE] * effects[DARK_DMG];
      case DamageType::PHYSICAL:
        return effects[WEAPON_DAMAGE];
      case DamageType::TRUE_DMG:
        return effects[WEAPON_DAMAGE];
    }
  }
  inline float get_max_health() const noexcept {
    return effects[MAX_HEALTH] * (1 + effects[HEALTH_MULT_P]);
  }
  inline float get_max_mana() const noexcept {
    return effects[MAX_MANA] * (1 + effects[MANA_MULT_P]);
  }
  inline void take_damage(const DamageStats& stats) {
    float& armour = effects[ARMOUR];
    float& armour_mult = effects[ARMOUR_MULT_P];
    float& shield = effects[SHIELD];

    float total_damage = stats.damage;

    if (stats.damage_type == DamageType::PHYSICAL) {
      total_damage *= (armour * (1 + armour_mult)) / (level * 50);
    } else if (stats.damage_type != DamageType::TRUE_DMG) {
      if (shield >= total_damage) {
        shield -= total_damage;
        return;
      } else {
        total_damage -= shield;
        shield = 0;
      }
    }

    health -= total_damage;
  }
  inline void refill_stats() noexcept {
    mana = get_max_mana();
    health = get_max_health();
  }
  inline float get_speed() const noexcept {
    return speed * (1 + effects[SPEED_MULT_P]);
  }
};

#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
