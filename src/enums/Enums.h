#ifndef DND_SRC_ENUMS_ENUMS_H_
#define DND_SRC_ENUMS_ENUMS_H_

#include <unordered_map>

enum class GameState { MainMenu, GameMenu, Game };
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

enum ShapeType { CIRCLE, RECT, TRIANGLE, POLYGON };
enum class DamageType { FIRE, POISON, ICE, EARTH, LIGHTNING };
inline static std::unordered_map<Class, std::string> classToString = {
    {Class::BARBARIAN, "Barbarian"}, {Class::BARD, "Bard"},       {Class::CLERIC, "Cleric"},
    {Class::DRUID, "Druid"},         {Class::FIGHTER, "Fighter"}, {Class::MONK, "Monk"},
    {Class::PALADIN, "Paladin"},     {Class::RANGER, "Ranger"},   {Class::ROGUE, "Rogue"},
    {Class::SORCERER, "Sorcerer"},   {Class::WARLOCK, "Warlock"}, {Class::WIZARD, "Wizard"}};
#endif  //DND_SRC_ENUMS_ENUMS_H_
