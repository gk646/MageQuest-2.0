#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

struct EffectStats {
  float effects[50] = {0};
};
struct SkillStats {
  float cool_down = 0;
  float mana_cost = 0;
  float health_cost = 0;

  inline float get_cd(float cd_percent) const { return cool_down - cool_down * (cd_percent / 100); }
};

struct DamageStats {
  SourceType damage_type = SourceType::MAGICAL;
  MagicType magic_type = MagicType::FIRE;
  float damage = 1;
  inline bool operator==(const DamageStats& d) const {
    return damage_type == d.damage_type && magic_type == d.magic_type && damage == d.damage;
  }
};

struct ArmourStats {
  float physical_armour = 0;
  float magical_armour = 0;

  float base_physical_armour = 0;
  float base_magical_armour = 0;

  inline void reset_to_base() {
    physical_armour = base_physical_armour;
    magical_armour = base_magical_armour;
  }
};

struct CombatStats {
  //Percent Values
  int16_t cooldown_reduction = 0;
  int16_t health_cost_reduction = 0;
  int16_t mana_cost_reduction = 0;
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
  ArmourStats armour_stats;
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
  inline void take_damage(const DamageStats& stats) {
    if (stats.damage_type == SourceType::MAGICAL) {
      health -= stats.damage * (1 - armour_stats.magical_armour / 100);
    } else if (stats.damage_type == SourceType::PHYSICAL) {
      health -= stats.damage * (1 - armour_stats.physical_armour / 100);
    }
  }
  inline void reset_to_base() {
    armour_stats.reset_to_base();

    speed = base_speed;
    max_health = base_max_health;
    max_mana = base_max_mana;
    max_concentration = base_max_concentration;
  }
  inline void update() {
    if (mana < max_mana) {
      mana += mana_regen / 60;
    }
    if (health < max_health) {
      health += health_regen / 60;
    }
  }
  inline void apply_monster_scaling(int level) {
    max_health *= std::pow(1.2F, level);
    max_health* DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY];
    health = max_health;
  }
};

struct EntityStats {
  Abilities abilities;
  GeneralStats general;
  CombatStats combat_stats;
  int level = 1;
  EntityStats() = default;
  EntityStats(float base_health, int level) : level(level) {
    general.max_health = base_health;
    general.apply_monster_scaling(level);
  }
  inline void reset_to_base() {
    abilities.reset_to_base();
    general.reset_to_base();
  }
  inline bool skill_useable(const SkillStats& stats) const noexcept {
    return stats.health_cost * (1 - combat_stats.health_cost_reduction / 100.0F) <=
               general.health &&
           stats.mana_cost * (1 - combat_stats.mana_cost_reduction / 100.0F) <= general.mana;
  }
  inline void use_skill(const SkillStats& stats) noexcept {
    general.mana -= (stats.mana_cost * (1 - combat_stats.mana_cost_reduction / 100.0F));
    general.health -= (stats.health_cost * (1 - combat_stats.health_cost_reduction / 100.0F));
  }
  inline void apply_flat(const EffectStats& stats) {}
};

#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
