#ifndef MAGE_QUEST_SRC_ENUMS_ENUMS_H_
#define MAGE_QUEST_SRC_ENUMS_ENUMS_H_

enum class GameState { MainMenu, GameMenu, Game, Loading, GameOver };
enum class MenuState { Main, Settings, ServerBrowser, HostGame };
enum class Class {
  BARBARIAN,
  BARD,
  CLERIC,
  DRUID,
  FIGHTER,
  MONK,
  PALADIN,
  RANGER,
  ROGUE,
  SORCERER,
  WARLOCK,
  WIZARD
};
enum class StatusEffectType { BUFF, DE_BUFF };
enum class ShapeType { CIRCLE, RECT, TRIANGLE, POLYGON };
enum class DamageType { FIRE, POISON, ICE, ARCANE, DARK, PHYSICAL, TRUE_DMG };
enum class ProjectileType { CONTINUOUS, ONE_HIT };
enum class Difficulty { EASY, NORMAL, HARD, TORMENT, TORMENT_2, TORMENT_3 };

inline Difficulty GAME_DIFFICULTY = Difficulty::NORMAL;

inline static std::unordered_map<Difficulty, float> DIFFICULTY_HEALTH_MULT = {
    {Difficulty::EASY, 0.5F}, {Difficulty::NORMAL, 1},    {Difficulty::HARD, 1.5F},
    {Difficulty::TORMENT, 3}, {Difficulty::TORMENT_2, 5}, {Difficulty::TORMENT_3, 10}};

enum class ItemType {
  HEAD = 5,
  CHEST = 4,
  PANTS = 8,
  BOOTS = 3,
  AMULET = 1,
  RING = 10,
  RELIC = 9,
  ONE_HAND = 7,
  TWO_HAND = 11,
  OFF_HAND = 6,
  BAG = 2,
  EMPTY = 0,
  MISC = 12
};
enum class ItemRarity {
  NORMAL = 1,
  RARE = 2,
  EPIC = 3,
  LEGENDARY = 4,
  SET_ITEM = 5,
  UNIQUE = 10
};
enum class Zone {
  Woodland_Edge,
  EtherRealm,
  DeadPlains,
  GrassLands,
  City1,
  Ruin_Dungeon,
  Hillcrest,
  Treasure_Cave,
  Hillcrest_Mountain_Cave,
  The_Grove,
  TestRoom,
  Goblin_Cave,
  Hillcrest_Hermit_Cave
};
//_P = percentile => 0.5, 0.05
enum Stat {
  STRENGTH,
  INTELLIGENCE,
  WISDOM,
  VITALITY,
  AGILITY,
  FOCUS,
  LUCK,
  CHARISMA,
  ENDURANCE,
  ARCANE_DMG_P,
  DARK_DMG_P,
  POISON_DMG_P,
  FIRE_DMG_P,
  ICE_DMG_P,
  DOT_LENGTH_P,
  DOT_DAMAGE_P,
  CRIT_DAMAGE_P,
  BUFF_LENGTH_P,
  SPEED_MULT_P,
  MANA_COST_REDUCTION_P,
  CDR_P,
  ARMOUR_MULT_P,
  HEALTH_MULT_P,
  MANA_MULT_P,
  WEAPON_DAMAGE,
  ARMOUR,
  MAX_SHIELD,
  MAGIC_FIND,
  MAX_HEALTH,
  MAX_MANA,
  CRIT_CHANCE,
  HEALTH_REGEN,
  MANA_REGEN,
  STATS_ENDING
};
inline std::unordered_map<Stat, std::string> statToName = {
    {ARCANE_DMG_P, "Arcane Damage (%)"},
    {DARK_DMG_P, "Dark Damage (%)"},
    {POISON_DMG_P, "Poison Damage (%)"},
    {FIRE_DMG_P, "Fire Damage (%)"},
    {ICE_DMG_P, "Ice Damage (%)"},
    {DOT_LENGTH_P, "DoT Length (%)"},
    {DOT_DAMAGE_P, "DoT Damage (%)"},
    {CRIT_CHANCE, "Crit Chance (%)"},
    {CRIT_DAMAGE_P, "Crit Damage (%)"},
    {STRENGTH, "Strength"},
    {INTELLIGENCE, "Intelligence"},
    {WISDOM, "Wisdom"},
    {VITALITY, "Vitality"},
    {AGILITY, "Agility"},
    {FOCUS, "Focus"},
    {LUCK, "Luck"},
    {CHARISMA, "Charisma"},
    {ENDURANCE, "Endurance"},
    {BUFF_LENGTH_P, "Buff Length (%)"},
    {HEALTH_REGEN, "Health Regen (%)"},
    {MANA_REGEN, "Mana Regen (%)"},
    {MANA_COST_REDUCTION_P, "Mana Cost Reduction (%)"},
    {CDR_P, "Cooldown Reduction (%)"},
    {ARMOUR_MULT_P, "Armour (%)"},
    {MAGIC_FIND, "Magic Find"},
    {MAX_HEALTH, "Max Health"},
    {MAX_MANA, "Max Mana"},
    {WEAPON_DAMAGE, "Weapon Damage"},
    {HEALTH_MULT_P, "Health (%)"},
    {MANA_MULT_P, "Mana (%)"},
    {ARMOUR, "Armour"},
    {MAX_SHIELD, "Max Shield"},
    {SPEED_MULT_P, "Speed (%)"},
};

inline static std::unordered_map<Zone, std::string> zoneMap = {
    {Zone::Woodland_Edge, "Woodland Edge"},
    {Zone::EtherRealm, "EtherRealm"},
    {Zone::DeadPlains, "DeadPlains"},
    {Zone::GrassLands, "GrassLands"},
    {Zone::City1, "City1"},
    {Zone::Ruin_Dungeon, "Ruin Dungeon"},
    {Zone::Hillcrest, "Hillcrest"},
    {Zone::Treasure_Cave, "Treasure Cave"},
    {Zone::Hillcrest_Mountain_Cave, "Hillcrest Mountain Cave"},
    {Zone::The_Grove, "The Grove"},
    {Zone::TestRoom, "TestRoom"},
    {Zone::Goblin_Cave, "Goblin Cave"},
    {Zone::Hillcrest_Hermit_Cave, "Hillcrest Hermit Cave"}};

inline std::unordered_map<Class, std::string> classToString = {
    {Class::BARBARIAN, "Barbarian"}, {Class::BARD, "Bard"},
    {Class::CLERIC, "Cleric"},       {Class::DRUID, "Druid"},
    {Class::FIGHTER, "Fighter"},     {Class::MONK, "Monk"},
    {Class::PALADIN, "Paladin"},     {Class::RANGER, "Ranger"},
    {Class::ROGUE, "Rogue"},         {Class::SORCERER, "Sorcerer"},
    {Class::WARLOCK, "Warlock"},     {Class::WIZARD, "Wizard"}};

inline std::unordered_map<std::string, Stat> attrToStat = {
    {"FOC", FOCUS},        {"END", ENDURANCE}, {"STR", STRENGTH},
    {"INT", INTELLIGENCE}, {"WIS", WISDOM},    {"CHA", CHARISMA},
    {"VIT", VITALITY},     {"AGI", AGILITY},   {"LUC", LUCK},
};
inline std::unordered_map<ItemRarity, Color> rarity_to_color = {
    {ItemRarity::NORMAL, Colors::NormalGrey},
    {ItemRarity::RARE, Colors::rareColor},
    {ItemRarity::EPIC, Colors::epicColor},
    {ItemRarity::LEGENDARY, Colors::legendaryColor},
    {ItemRarity::SET_ITEM, Colors::setItem},
    {ItemRarity::UNIQUE, Colors::highQuality},
};
#endif  //MAGE_QUEST_SRC_ENUMS_ENUMS_H_
