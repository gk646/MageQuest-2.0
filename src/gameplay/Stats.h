#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

#include <cstdint>
#include "../util/Enums.h"


struct AbilityStats{
  int cool_down;
  float mana_cost;
  float health_cost;
};

struct DamageStats {
  DamageType damage_type;
  MagicType magic_type;
  ProjectileType projectile_type;
  float damage;

  DamageStats()
      : damage_type(DamageType::PHYSICAL),
        magic_type(MagicType::FIRE),
        damage(1),
        projectile_type(ProjectileType::ONE_HIT) {}

  DamageStats(DamageType damage_type, MagicType magic_type, ProjectileType projectile_type,
              float damage)
      : damage_type(damage_type),
        magic_type(magic_type),
        projectile_type(projectile_type),
        damage(damage) {}
};
struct ArmourStats {
  float physical_armour;
  float magical_armour;

  ArmourStats() : physical_armour(0), magical_armour(0) {}
  ArmourStats(float physical_armour, float magical_armour)
      : physical_armour(physical_armour), magical_armour(magical_armour) {}

   float get_damage(DamageStats stats) const {
    if (stats.damage_type == DamageType::MAGICAL) {
      return stats.damage * (1 - magical_armour / 100);
    }else if(stats.damage_type == DamageType::PHYSICAL){
      return stats.damage * (1 - physical_armour / 100);
    }
  }
};

//Percent Values
struct CombatStats{
  float cooldown_reduction;

};

struct Abilities {
  int8_t strength;
  int8_t wisdom;
  int8_t dexterity;
  int8_t intelligence;
  int8_t charisma;
  int8_t constitution;

  Abilities()
      : strength(0), wisdom(0), dexterity(0), intelligence(0), charisma(0), constitution(0) {}

  Abilities(int8_t strength, int8_t wisdom, int8_t dexterity, int8_t intelligence, int8_t charisma,
            int8_t constitution)
      : strength(strength),
        wisdom(wisdom),
        dexterity(dexterity),
        intelligence(intelligence),
        charisma(charisma),
        constitution(constitution) {}
};

struct GeneralStats {
  float mana;
  float concentration;
  float health;
  float speed;

  float base_speed;
  float max_health;
  float max_mana;
  float max_concentration;

  GeneralStats() : mana(0), concentration(0), health(10), max_health(10), speed(5) {}

  GeneralStats(float mana, float concentration, float health, float speed = 5)
      : mana(mana),
        concentration(concentration),
        health(health),
        max_health(health),
        speed(speed) {}
};

struct EntityStats {
  ArmourStats armour_stats;
  Abilities abilities;
  GeneralStats general;
  CombatStats combat_stats;

  EntityStats() : abilities{}, general{} {}

  EntityStats(const Abilities& abilities, const GeneralStats& general)
      : abilities(abilities), general(general) {}
};

#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
