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
enum Zone {
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

std::unordered_map<std::string, Zone> zoneMap = {
    {"Woodland Edge", Woodland_Edge},
    {"EtherRealm", EtherRealm},
    {"DeadPlains", DeadPlains},
    {"GrassLands", GrassLands},
    {"City1", City1},
    {"Ruin Dungeon", Ruin_Dungeon},
    {"Hillcrest", Hillcrest},
    {"Treasure Cave", Treasure_Cave},
    {"Hillcrest Mountain Cave", Hillcrest_Mountain_Cave},
    {"The Grove", The_Grove},
    {"TestRoom", TestRoom},
    {"Goblin Cave", Goblin_Cave},
    {"Hillcrest Hermit Cave", Hillcrest_Hermit_Cave}};
inline static std::unordered_map<Class, std::string> classToString = {
    {Class::BARBARIAN, "Barbarian"}, {Class::BARD, "Bard"},       {Class::CLERIC, "Cleric"},
    {Class::DRUID, "Druid"},         {Class::FIGHTER, "Fighter"}, {Class::MONK, "Monk"},
    {Class::PALADIN, "Paladin"},     {Class::RANGER, "Ranger"},   {Class::ROGUE, "Rogue"},
    {Class::SORCERER, "Sorcerer"},   {Class::WARLOCK, "Warlock"}, {Class::WIZARD, "Wizard"}};

#endif  //MAGE_QUEST_SRC_ENUMS_ENUMS_H_
