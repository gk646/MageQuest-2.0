#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_

#include <cstdint>
#include "../../../enums/Enums.h"

struct DamageStats {
  DamageType damage_type;
  float damage;

  DamageStats() : damage_type(DamageType{}), damage(1) {}

  DamageStats(DamageType damage_type, float damage) : damage_type(damage_type), damage(damage) {}
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
  float max_health;
  float speed;

  GeneralStats() : mana(0), concentration(0), health(10), max_health(10), speed(1) {}

  GeneralStats(float mana, float concentration, float health, float speed = 1)
      : mana(mana),
        concentration(concentration),
        health(health),
        max_health(health),
        speed(speed) {}
};

struct EntityStats {
  Abilities abilities;
  GeneralStats general;

  EntityStats() : abilities{}, general{} {}

  EntityStats(const Abilities& abilities, const GeneralStats& general)
      : abilities(abilities), general(general) {}
};

#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
