#ifndef MAGE_QUEST_SRC_ENUMS_ENUMS_H_
#define MAGE_QUEST_SRC_ENUMS_ENUMS_H_

/* |-----------------------------------------------------|
 * |                          SYSTEM                      |
 * |-----------------------------------------------------|
 */
enum class GameState { MainMenu, GameMenu, Game, Loading, GameOver };
enum class MenuState { Main, Settings, ServerBrowser, HostGame };
enum class MapLayer : uint8_t { BACK_GROUND, MIDDLE_GROUND, FRONT };

/* |-----------------------------------------------------|
 * |                    GAMEPLAY                         |
 * |-----------------------------------------------------|
 */
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

enum class MonsterType : uint8_t {
  ANY,
  BLOOD_HOUND,
  GHOST,
  SKEL_WAR,
  SKEL_SPEAR,
  WOLF,
  BOSS_DEATH_BRINGER,
  BOSS_KNIGHT,
  BOSS_SLIME,
  GOBLIN,
  KNIGHT,
  MUSHROOM,
  SKEL_ARCHER,
  SKEL_SHIELD,
  SNAKE
};

enum class EffectType : uint8_t { STUN, BURN, SLOW, ROOT };
enum class ShapeType : uint8_t { CIRCLE, RECT };
enum class DamageType : uint8_t { FIRE, POISON, ICE, ARCANE, DARK, PHYSICAL, TRUE_DMG };


inline static std::unordered_map<DamageType, Color> damageTypeToColor{
    {DamageType::FIRE, Colors::fire_red},
    {DamageType::ICE, Colors::frostBlue},
    {DamageType::POISON, Colors::poison_green},
    {DamageType::ARCANE, Colors::arcaneBlue},
    {DamageType::DARK, Colors::darkMagicPurple},
    {DamageType::PHYSICAL, Colors::physical_grey},
};
enum class HitType : uint8_t { CONTINUOUS, ONE_HIT, ONE_TICK };
enum class Difficulty : uint8_t { EASY, NORMAL, HARD, TORMENT, TORMENT_2, TORMENT_3 };
enum class TriggerSpreadType : uint8_t { MIXED_GOBLIN, MIX_ALL };
enum class TextAlign : uint8_t { LEFT, RIGHT, MIDDLE };

enum class ShadowType : uint16_t {
  TREE_GREEN_BUSH = 886,
  TREE_GREEN_POINTY = 890,
  TREE_GREEN_SWAMPY = 894,
  ROCK_SMALL_1 = 266,
  ROCK_SMALL_2 = 267,
  ROCK_SMALL_3 = 268,
  ROCK_SMALL_4 = 269,
  END
};

inline static int16_t shadowTileNumbers[(int)ShadowType::END] = {
    static_cast<int16_t>(ShadowType::TREE_GREEN_BUSH),
    static_cast<int16_t>(ShadowType::TREE_GREEN_POINTY),
    static_cast<int16_t>(ShadowType::TREE_GREEN_SWAMPY),
    static_cast<int16_t>(ShadowType::ROCK_SMALL_1),
    static_cast<int16_t>(ShadowType::ROCK_SMALL_2),
    static_cast<int16_t>(ShadowType::ROCK_SMALL_3),
    static_cast<int16_t>(ShadowType::ROCK_SMALL_4)};

std::unordered_map<ShadowType, Texture> shadowToTexture{
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()},

};
enum class Keybind : uint8_t {
  PLAYER_LIGHT,
  ABILITY_1,
  ABILITY_2,
  ABILITY_3,
  ABILITY_4,
  ABILITY_5,
  ABILITY_6,
  END
};
enum class ItemType : uint8_t {
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
enum class ItemRarity : uint8_t {
  NORMAL = 1,
  RARE = 2,
  EPIC = 3,
  LEGENDARY = 4,
  SET_ITEM = 5,
  UNIQUE = 10
};
enum class Zone : uint8_t {
  //only add to bottom as number is needed for saving
  Tutorial,
  EtherRealm,
  DeadPlains,
  GrassLands,
  City1,
  Ruin_Dungeon,
  Hillcrest,
  Hillcrest_Treasure_Cave,
  Hillcrest_Mountain_Cave,
  The_Grove,
  TestRoom,
  Goblin_Cave,
  Hillcrest_Hermit_Cave,
  ZONE_END
};
enum GameStatistic : uint8_t {
  PIXELS_WALKED,
  MONSTERS_KILLED,
  QUESTS_COMPLETED,
  COINS_EARNED,
  GAME_TICKS_PLAYED,
  BOSSES_KILLED,
  ITEMS_PICKED_UP,
  GAME_STATISTICS_END
};

//_P = percentile => 0.5, 0.05
enum Stat : uint8_t {
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
  XP_MODIFIER_P,
  CDR_P,
  ARMOUR_MULT_P,
  HEALTH_MULT_P,
  MANA_MULT_P,
  WEAPON_DAMAGE,  // the first after _P enums || other percentile only before mana mult
  DODGE_CHANCE,
  CRIT_CHANCE,
  ARMOUR,
  MAX_SHIELD,
  MAGIC_FIND,
  MAX_HEALTH,
  MAX_MANA,
  HEALTH_REGEN,
  MANA_REGEN,
  STATS_ENDING
};

/* |-----------------------------------------------------|
 * |                  QUESTS                             |
 * |-----------------------------------------------------|
 */

enum class NodeType : uint8_t {
  GOTO,
  KILL,
  SPAWN,
  SPEAK,
  COLLECT,
  SET_QUEST_SHOWN,
  REQUIRE,
  PROTECT,
  ESCORT,
  MIX,
  NPC_MOVE,
  NPC_SAY,
  NPC_SAY_PROXIMITY,
  CHOICE_DIALOGUE_SIMPLE,
  TILE_ACTION,
  PLAYER_THOUGHT
};
inline static std::unordered_map<std::string, NodeType> node_to_type = {
    {"SPAWN", NodeType::SPAWN},
    {"NPC_SAY", NodeType::NPC_SAY},
    {"TILE_ACTION", NodeType::TILE_ACTION},
    {"GOTO", NodeType::GOTO},
    {"KILL", NodeType::KILL},
    {"SPEAK", NodeType::SPEAK},
    {"COLLECT", NodeType::COLLECT},
    {"REQUIRE", NodeType::REQUIRE},
    {"PROTECT", NodeType::PROTECT},
    {"ESCORT", NodeType::ESCORT},
    {"MIX", NodeType::MIX},
    {"NPC_MOVE", NodeType::NPC_MOVE},
    {"NPC_SAY_PROXIMITY", NodeType::NPC_SAY_PROXIMITY},
    {"SET_QUEST_SHOWN", NodeType::SET_QUEST_SHOWN},
    {"PLAYER_THOUGHT", NodeType::PLAYER_THOUGHT},
    {"CHOICE_DIALOGUE_SIMPLE", NodeType::CHOICE_DIALOGUE_SIMPLE}};

enum class QuestState : uint8_t { IN_ACTIVE, ACTIVE, COMPLETED };
enum class Quest_ID : uint8_t { TUTORIAL, MARLA, END };
inline static std::unordered_map<std::string, Quest_ID> stringToQuestID = {
    {"TUTORIAL", Quest_ID::TUTORIAL},
    {"MARLA", Quest_ID::MARLA},
};
enum class NPC_ID : uint8_t {
  //only add to bottom as number is needed when saving
  DECKARD,
  MARLA,
  ARIA,
  HILLCREST_MAYOR,
  RECEPTIONS,
  DYING_MAN,
  NIETZSCHE,
  VILLAGER,
  TRADER,
  RANDOM,
  NPC_END
};
std::unordered_map<std::string, NPC_ID> npcIdMap = {
    {"DECKARD", NPC_ID::DECKARD},
    {"MARLA", NPC_ID::MARLA},
    {"ARIA", NPC_ID::ARIA},
    {"HILLCREST_MAYOR", NPC_ID::HILLCREST_MAYOR},
    {"RECEPTIONS", NPC_ID::RECEPTIONS},
    {"DYING_MAN", NPC_ID::DYING_MAN},
    {"NIETZSCHE", NPC_ID::NIETZSCHE},
    {"VILLAGER", NPC_ID::VILLAGER},
    {"TRADER", NPC_ID::TRADER},
    {"RANDOM", NPC_ID::RANDOM},
    {"END", NPC_ID::NPC_END}};

std::unordered_map<NPC_ID, std::string> npcIdToStringMap = {
    {NPC_ID::DECKARD, "Deckard"},
    {NPC_ID::MARLA, "Marla"},
    {NPC_ID::HILLCREST_MAYOR, "Hillcrest Mayor"},
    {NPC_ID::RECEPTIONS, "Receptionist"},
    {NPC_ID::DYING_MAN, "Dying Man"},
    {NPC_ID::NIETZSCHE, "Nietzsche"},
    {NPC_ID::VILLAGER, "Villager"},
    {NPC_ID::TRADER, "Trader"},
    {NPC_ID::RANDOM, "Person"}};

enum class MultiplayerType { CLIENT, SERVER, OFFLINE };
enum UDP_Channel : uint8_t {
  FILL,
  UDP_PLAYER_POS_UPDATE,
  UDP_PLAYER_POS_BROADCAST,
  UDP_MONSTER_DAMAGE,
  UDP_MONSTER_UPDATE,
  UDP_MONSTER_SPAWN,
  UDP_PROJECTILE,
  LAST_CHANNEL
};
inline static std::unordered_map<std::string, MonsterType> stringToMonsterID = {
    {"ANY", MonsterType::ANY},
    {"SKEL_WAR", MonsterType::SKEL_WAR},
    {"SKEL_SPEAR", MonsterType::SKEL_SPEAR},
    {"WOLF", MonsterType::WOLF},
    {"BOSS_DEATH_BRINGER", MonsterType::BOSS_DEATH_BRINGER},
    {"BOSS_KNIGHT", MonsterType::BOSS_KNIGHT},
    {"BOSS_SLIME", MonsterType::BOSS_SLIME},
    {"GOBLIN", MonsterType::GOBLIN},
    {"KNIGHT", MonsterType::KNIGHT},
    {"MUSHROOM", MonsterType::MUSHROOM},
    {"SKEL_ARCHER", MonsterType::SKEL_ARCHER},
    {"SKEL_SHIELD", MonsterType::SKEL_SHIELD},
    {"BLOOD_HOUND", MonsterType::BLOOD_HOUND},
    {"SNAKE", MonsterType::SNAKE}};

inline static std::unordered_map<std::string, TriggerSpreadType> stringToTriggerType = {
    {"mixedGoblin", TriggerSpreadType::MIXED_GOBLIN},
    {"mixeall", TriggerSpreadType::MIX_ALL},
};

enum ProjectileType : uint8_t {
  POISON_BALL,
  FIRE_STRIKE,
  FIRE_STRIKE_II,
  FIRE_BALL,
  ARROW_NORMAL,
  BLAST_HAMMER,
  ENERGY_SPHERE,
  FIRE_SWORD,
  FROST_NOVA,
  ICE_LANCE,
  INFERNO_RAY,
  LIGHTNING,
  PYRO_BLAST,
  SOLAR_FLARE,
  THUNDER_SPLASH,
  THUNDER_STRIKE,
  VOID_ERUPTION,
  VOID_FIELD,
  PROJECTILE_END
};
inline static std::unordered_map<std::string, ProjectileType> stringToProjectile = {
    {"POISON_BALL", ProjectileType::POISON_BALL},
    {"FIRE_STRIKE_II", ProjectileType::FIRE_STRIKE_II},
    {"FIRE_STRIKE", ProjectileType::FIRE_STRIKE},
    {"FIRE_BALL", ProjectileType::FIRE_BALL},
    {"BLAST_HAMMER", ProjectileType::BLAST_HAMMER},
    {"ENERGY_SPHERE", ProjectileType::ENERGY_SPHERE},
    {"FIRE_SWORD", ProjectileType::FIRE_SWORD},
    {"FROST_NOVA", ProjectileType::FROST_NOVA},
    {"ICE_LANCE", ProjectileType::ICE_LANCE},
    {"INFERNO_RAY", ProjectileType::INFERNO_RAY},
    {"LIGHTNING", ProjectileType::LIGHTNING},
    {"PYRO_BLAST", ProjectileType::PYRO_BLAST},
    {"SOLAR_FLARE", ProjectileType::SOLAR_FLARE},
    {"THUNDER_SPLASH", ProjectileType::THUNDER_SPLASH},
    {"THUNDER_STRIKE", ProjectileType::THUNDER_STRIKE},
    {"VOID_ERUPTION", ProjectileType::VOID_ERUPTION},
    {"VOID_FIELD", ProjectileType::VOID_FIELD}};

inline static std::unordered_map<Difficulty, float> DIFFICULTY_HEALTH_MULT = {
    {Difficulty::EASY, 0.5F}, {Difficulty::NORMAL, 1},    {Difficulty::HARD, 1.5F},
    {Difficulty::TORMENT, 3}, {Difficulty::TORMENT_2, 5}, {Difficulty::TORMENT_3, 10}};

inline std::unordered_map<Stat, std::string> statToName = {
    {ARCANE_DMG_P, "Arcane Damage (%)"},
    {DARK_DMG_P, "Dark Damage (%)"},
    {POISON_DMG_P, "Poison Damage (%)"},
    {FIRE_DMG_P, "Fire Damage (%)"},
    {ICE_DMG_P, "Ice Damage (%)"},
    {DOT_LENGTH_P, "DoT Length"},
    {DOT_DAMAGE_P, "DoT Damage"},
    {CRIT_CHANCE, "Crit Chance"},
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
    {HEALTH_REGEN, "Health Regen"},
    {MANA_REGEN, "Mana Regen"},
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
    {DODGE_CHANCE, "Dodge Chance"}};

inline static std::unordered_map<Zone, std::string> zoneMap = {
    {Zone::Tutorial, "Woodland Edge"},
    {Zone::EtherRealm, "EtherRealm"},
    {Zone::DeadPlains, "DeadPlains"},
    {Zone::GrassLands, "GrassLands"},
    {Zone::City1, "City1"},
    {Zone::Ruin_Dungeon, "Ruin Dungeon"},
    {Zone::Hillcrest, "Hillcrest"},
    {Zone::Hillcrest_Treasure_Cave, "Treasure Cave"},
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
inline std::unordered_map<ItemType, std::string> type_to_string = {
    {ItemType::HEAD, "Head"},         {ItemType::CHEST, "Chest"},
    {ItemType::PANTS, "Pants"},       {ItemType::BOOTS, "Boots"},
    {ItemType::AMULET, "Amulet"},     {ItemType::RING, "Ring"},
    {ItemType::RELIC, "Relic"},       {ItemType::ONE_HAND, "One Hand"},
    {ItemType::TWO_HAND, "Two Hand"}, {ItemType::OFF_HAND, "Off Hand"},
    {ItemType::BAG, "Bag"},           {ItemType::EMPTY, "Empty"},
    {ItemType::MISC, "Misc"}};

inline std::unordered_map<ItemRarity, Color> rarity_to_color = {
    {ItemRarity::NORMAL, Colors::NormalGrey},
    {ItemRarity::RARE, Colors::rareColor},
    {ItemRarity::EPIC, Colors::epicColor},
    {ItemRarity::LEGENDARY, Colors::legendaryColor},
    {ItemRarity::SET_ITEM, Colors::setItem},
    {ItemRarity::UNIQUE, Colors::highQuality},
};
inline static std::unordered_map<std::string, Zone> stringToZoneMap = {
    {"Tutorial", Zone::Tutorial},
    {"EtherRealm", Zone::EtherRealm},
    {"DeadPlains", Zone::DeadPlains},
    {"GrassLands", Zone::GrassLands},
    {"City1", Zone::City1},
    {"TutorialDungeon", Zone::Ruin_Dungeon},
    {"Hillcrest", Zone::Hillcrest},
    {"Hillcrest_Treasure_Cave", Zone::Hillcrest_Treasure_Cave},
    {"Hillcrest_Mountain_Cave", Zone::Hillcrest_Mountain_Cave},
    {"TheGrove", Zone::The_Grove},
    {"TestRoom", Zone::TestRoom},
    {"Goblin_Cave", Zone::Goblin_Cave},
    {"Hillcrest_Hermit_Cave", Zone::Hillcrest_Hermit_Cave}};

inline Difficulty GAME_DIFFICULTY = Difficulty::NORMAL;
#endif  //MAGE_QUEST_SRC_ENUMS_ENUMS_H_
