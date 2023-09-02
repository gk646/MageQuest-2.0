#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_


struct SkillStats {
  int cool_down = 0;
  float mana_cost = 1;
  float health_cost = 0;
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

//Percent Values
struct CombatStats {
  int16_t cooldown_reduction = 0;
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

  float speed = 3;
  float max_health = 10;
  float max_mana = 20;
  float max_concentration = 10;

  float base_speed = 3;
  float base_max_health = 10;
  float base_max_mana = 20;
  float base_max_concentration = 10;

  inline void take_damage(DamageStats stats) {
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
};

struct EntityStats {
  Abilities abilities;
  GeneralStats general;
  CombatStats combat_stats;

  inline void reset_to_base() {
    abilities.reset_to_base();
    general.reset_to_base();
  }
};

#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
