#ifndef MAGE_QUEST_SRC_ENUMS_ENUMS_H_
#define MAGE_QUEST_SRC_ENUMS_ENUMS_H_

enum class GameState { MainMenu, GameMenu, Game, Loading };
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
enum class MagicType { FIRE, POISON, ICE, EARTH, LIGHTNING, DARK };
enum class SourceType { PHYSICAL, MAGICAL };
enum class ProjectileType { CONTINUOUS, ONE_HIT };
enum class Difficulty { EASY, NORMAL, HARD, TORMENT, TORMENT_2, TORMENT_3 };
enum class ItemType{HEAD, CHEST, PANTS, BOOTS, AMULET, RING, RELIC, ONE_HAND, TWO_HAND, OFF_HAND, BAG};
enum class ItemRarity{NORMAL, RARE, EPIC, LEGENDARY, SET_ITEM, UNIQUE};
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
    {Zone::Hillcrest_Hermit_Cave, "Hillcrest Hermit Cave"}
};

inline static std::unordered_map<Class, std::string> classToString = {
    {Class::BARBARIAN, "Barbarian"}, {Class::BARD, "Bard"},       {Class::CLERIC, "Cleric"},
    {Class::DRUID, "Druid"},         {Class::FIGHTER, "Fighter"}, {Class::MONK, "Monk"},
    {Class::PALADIN, "Paladin"},     {Class::RANGER, "Ranger"},   {Class::ROGUE, "Rogue"},
    {Class::SORCERER, "Sorcerer"},   {Class::WARLOCK, "Warlock"}, {Class::WIZARD, "Wizard"}};

#endif  //MAGE_QUEST_SRC_ENUMS_ENUMS_H_
