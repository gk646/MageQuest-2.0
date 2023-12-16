#ifndef MAGE_QUEST_SRC_ENUMS_ENUMS_H_
#define MAGE_QUEST_SRC_ENUMS_ENUMS_H_

enum class GameState : uint8_t { MainMenu, GameMenu, Game, Loading, GameOver };
enum class MenuState : uint8_t { Main, Settings, ServerBrowser, HostGame, Transition };
enum class MapLayer : uint8_t { BACK_GROUND, MIDDLE_GROUND, FRONT };
enum class Class : uint8_t {
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
  RAT,
  FLYING_EYE,
  BOSS_DEATH_BRINGER,
  BOSS_ANCIENT_GOLEM,
  BOSS_STONE_KNIGHT,
  BOSS_SLIME,
  GOBLIN,
  SKULL_WOLF,
  KNIGHT,
  MUSHROOM,
  SKEL_ARCHER,
  SKEL_SHIELD,
  SNAKE
};

inline static bool IsBoss(const MonsterType type) {
  return type == MonsterType::BOSS_STONE_KNIGHT ||
         type == MonsterType::BOSS_ANCIENT_GOLEM ||
         type == MonsterType::BOSS_DEATH_BRINGER || type == MonsterType::BOSS_SLIME;
}

enum class EffectType : uint8_t {
  STUN,
  BURN,
  BLEED,
  SLOW,
  BERSERK,
  REGENERATION,
  SWIFTNESS,
  RESISTANCE,
  POISON,
  ROOT,
  SPELL_ECHO_CD,
  ELEMENTAL_EQUILIBRIUM_BUFF,
  ELEMENTAL_EQUILIBRIUM_CD
};
struct EffectInfo {
  std::string name;
  std::string description;
  Texture icon;
};
std::unordered_map<std::string, EffectType> effectMap = {
    {"STUN", EffectType::STUN},
    {"BURN", EffectType::BURN},
    {"SLOW", EffectType::SLOW},
    {"SWIFTNESS", EffectType::SWIFTNESS},
    {"BERSERK", EffectType::BERSERK},
    {"BLEED", EffectType::BLEED},
    {"RESISTANCE", EffectType::RESISTANCE},
    {"ROOT", EffectType::ROOT},
    {"REGENERATION", EffectType::REGENERATION},
    {"POISON", EffectType::POISON},
    {"SPELL_ECHO_CD", EffectType::SPELL_ECHO_CD},
    {"ELEMENTAL_EQUILIBRIUM_BUFF", EffectType::ELEMENTAL_EQUILIBRIUM_BUFF},
    {"ELEMENTAL_EQUILIBRIUM_CD", EffectType::ELEMENTAL_EQUILIBRIUM_CD}};

enum class ShapeType : uint8_t { CIRCLE, RECT };
enum class DamageType : uint8_t {
  ARCANE = 9,
  POISON = 11,
  FIRE = 12,
  ICE = 13,
  DARK,
  PHYSICAL,
  TRUE_DMG
};
std::unordered_map<EffectType, EffectInfo> effectToInfo{};
std::unordered_map<DamageType, std::string> damageTypeNames = {
    {DamageType::ARCANE, "arcane"},       {DamageType::POISON, "poison"},
    {DamageType::FIRE, "fire"},           {DamageType::ICE, "ice"},
    {DamageType::DARK, "dark"},           {DamageType::PHYSICAL, "physical"},
    {DamageType::TRUE_DMG, "true damage"}};
std::unordered_map<std::string, DamageType> damageNamesToType = {
    {"arcane", DamageType::ARCANE},       {"poison", DamageType::POISON},
    {"fire", DamageType::FIRE},           {"ice", DamageType::ICE},
    {"dark", DamageType::DARK},           {"physical", DamageType::PHYSICAL},
    {"true damage", DamageType::TRUE_DMG}};

inline static DamageType commonSchools[4] = {DamageType::FIRE, DamageType::ICE,
                                             DamageType::ARCANE, DamageType::POISON};

inline static std::unordered_map<DamageType, Color> damageTypeToColor{
    {DamageType::FIRE, Colors::fire_red},
    {DamageType::ICE, Colors::frostBlue},
    {DamageType::POISON, Colors::poison_green},
    {DamageType::ARCANE, Colors::arcaneBlue},
    {DamageType::DARK, Colors::darkMagicPurple},
    {DamageType::TRUE_DMG, Colors::questMarkerYellow},
    {DamageType::PHYSICAL, Colors::physical_grey},
};
enum class HitType : uint8_t { CONTINUOUS, ONE_HIT, ONE_TICK };
enum class Difficulty : uint8_t { EASY, NORMAL, HARD, TORMENT };
enum class TriggerSpreadType : uint8_t { MIXED_GOBLIN, MIX_ALL };
enum class Alignment : uint8_t { LEFT, RIGHT, MIDDLE };

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

//Textures are loaded later // dummies for now
std::unordered_map<ShadowType, Texture> shadowToTexture{
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()}, {ShadowType::TREE_GREEN_BUSH, Texture()},
    {ShadowType::TREE_GREEN_BUSH, Texture()},

};
enum class TalentSize : uint8_t { NORMAL, MID, BIG };

enum  Keybind : uint8_t {
  KB_PLAYER_LIGHT,
  KB_ABILITY_1,
  KB_ABILITY_2,
  KB_ABILITY_3,
  KB_ABILITY_4,
  KB_ABILITY_5,
  KB_ABILITY_6,
  KB_END
};
enum class StatusMessageType {
  OUT_OF_RANGE,
  NOT_ENOUGH_MANA,
  NOT_IN_SIGHT,
  FAILED_EQUIP,
  FAILED_ITEM_DROP
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
enum class ItemSetNum : int8_t { NO_SET = -1, ROMEO_JULIET, DEVILS_AVATAR, END };
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
  Oasis,
  Oasis_Cave,
  NO_ZONE,
  ZONE_END
};

inline static bool IsCave(Zone zone) noexcept {
  return zone == Zone::Ruin_Dungeon || zone == Zone::Hillcrest_Treasure_Cave ||
         zone == Zone::Hillcrest_Mountain_Cave || zone == Zone::Hillcrest_Hermit_Cave ||
         zone == Zone::Goblin_Cave || zone == Zone::Oasis_Cave;
}

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
  //always add at bottom -> before WEAPON_DAMAGE or before BAG_SLOTS
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
  MANA_REGEN_MULT_P,
  HEALTH_REGEN_MULT_P,
  ARCANE_DMG_WEAKNESS_P,
  DARK_DMG_WEAKNESS_P,
  POISON_DMG_WEAKNESS_P,
  FIRE_DMG_WEAKNESS_P,
  ICE_DMG_WEAKNESS_P,
  ARCANE_COST_REDUCTION_P,
  DARK_COST_REDUCTION_P,
  POISON_COST_REDUCTION_P,
  FIRE_COST_REDUCTION_P,
  ICE_COST_REDUCTION_P,
  DAMAGE_RESISTANCE_P,  //Percent reduction of total damage received
  GOLD_PICKUP_P,
  FILL8,
  FILL9,
  FILL0,
  FILL10,
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
  FILL1,
  FILL2,
  FILL3,
  FILL4,
  FILL5,
  FILL11,
  BAG_SLOTS,  //always second last
  STATS_ENDING
};

inline static bool IsPercentile(const Stat stat) noexcept {
  return stat >= 9 && stat < 43;
}
inline static bool IsPercentStat(const Stat stat) noexcept {
  return true;
}
/* |-----------------------------------------------------|
 * |                  QUESTS                             |
 * |-----------------------------------------------------|
 */

enum class NodeType : uint8_t {
  GOTO,
  KILL,
  SPAWN,
  SPEAK,
  SET_QUEST_ZONE,
  SCRIPTED_NODE,
  COLLECT,
  COMBAT_TRIGGER,
  REMOVE_ITEM,
  SET_QUEST_SHOWN,
  SWITCH_ALTERNATIVE,
  SET_NPC_POS,
  REQUIRE,
  FINISH_QUEST,
  DESPAWN_NPC,
  WAIT,
  PROTECT,
  CHOICE_DIALOGUE,
  ESCORT,
  MIX,
  NPC_MOVE,
  NPC_SAY,
  NPC_SAY_PROXIMITY,
  CHOICE_DIALOGUE_SIMPLE,
  TILE_ACTION,
  PLAYER_THOUGHT,
  OPTIONAL_POSITION,
  SKIP
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
    {"COMBAT_TRIGGER", NodeType::COMBAT_TRIGGER},
    {"FINISH_QUEST", NodeType::FINISH_QUEST},
    {"SET_QUEST_ZONE", NodeType::SET_QUEST_ZONE},
    {"DESPAWN_NPC", NodeType::DESPAWN_NPC},
    {"REMOVE_ITEM", NodeType::REMOVE_ITEM},
    {"SET_NPC_POS", NodeType::SET_NPC_POS},
    {"SCRIPTED_NODE", NodeType::SCRIPTED_NODE},
    {"WAIT", NodeType::WAIT},
    {"SWITCH_ALTERNATIVE", NodeType::SWITCH_ALTERNATIVE},
    {"PROTECT", NodeType::PROTECT},
    {"CHOICE_DIALOGUE", NodeType::CHOICE_DIALOGUE},
    {"ESCORT", NodeType::ESCORT},
    {"MIX", NodeType::MIX},
    {"NPC_MOVE", NodeType::NPC_MOVE},
    {"NPC_SAY_PROXIMITY", NodeType::NPC_SAY_PROXIMITY},
    {"SET_QUEST_SHOWN", NodeType::SET_QUEST_SHOWN},
    {"PLAYER_THOUGHT", NodeType::PLAYER_THOUGHT},
    {"OPTIONAL_POSITION", NodeType::OPTIONAL_POSITION},
    {"SKIP", NodeType::SKIP},
    {"CHOICE_DIALOGUE_SIMPLE", NodeType::CHOICE_DIALOGUE_SIMPLE}};

enum class QuestState : uint8_t { IN_ACTIVE, ACTIVE, COMPLETED };
enum class TextSource : uint8_t { NPC, MONSTER, PLAYER };

enum class Quest_ID : uint8_t {
  START_SOMETHING_NEW = 1,
  TUTORIAL,
  MARLA_QUEST,
  HILLCREST_PUZZLE,
  END
};

inline static std::unordered_map<std::string, Quest_ID> stringToQuestID = {
    {"TUTORIAL", Quest_ID::TUTORIAL},
    {"MARLA_QUEST", Quest_ID::MARLA_QUEST},
    {"START_SOMETHING_NEW", Quest_ID::START_SOMETHING_NEW},
    {"HILLCREST_PUZZLE", Quest_ID::HILLCREST_PUZZLE},
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
  SATIRO,
  NPC_END
};
enum class WorldObjectType : int16_t {
  BASIC_CHEST = 7176,
  NORMAL_CHEST = 137,
  EPIC_CHEST = 2191,
  RARE_FLOWER
};
inline static std::unordered_set<int16_t> worldObjectTable{
    (int16_t)WorldObjectType::BASIC_CHEST, (int16_t)WorldObjectType::NORMAL_CHEST,
    (int16_t)WorldObjectType::EPIC_CHEST};

std::unordered_map<std::string, NPC_ID> stringToNPCID = {
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
    {"SATIRO", NPC_ID::SATIRO},
    {"END", NPC_ID::NPC_END}};

std::unordered_map<NPC_ID, std::string> npcIdToStringMap = {
    {NPC_ID::DECKARD, "Deckard"},
    {NPC_ID::MARLA, "Marla"},
    {NPC_ID::ARIA, "Aria"},
    {NPC_ID::HILLCREST_MAYOR, "Hillcrest Mayor"},
    {NPC_ID::RECEPTIONS, "Receptionist"},
    {NPC_ID::DYING_MAN, "Dying Man"},
    {NPC_ID::NIETZSCHE, "Nietzsche"},
    {NPC_ID::VILLAGER, "Villager"},
    {NPC_ID::TRADER, "Trader"},
    {NPC_ID::RANDOM, "Person"},
    {NPC_ID::SATIRO, "Satiro"}};

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
    {"SKULL_WOLF", MonsterType::SKULL_WOLF},
    {"WOLF", MonsterType::WOLF},
    {"FLYING_EYE", MonsterType::FLYING_EYE},
    {"GHOST", MonsterType::GHOST},
    {"BOSS_DEATH_BRINGER", MonsterType::BOSS_DEATH_BRINGER},
    {"BOSS_STONE_KNIGHT", MonsterType::BOSS_STONE_KNIGHT},
    {"BOSS_STONE_GOLEM", MonsterType::BOSS_ANCIENT_GOLEM},
    {"BOSS_SLIME", MonsterType::BOSS_SLIME},
    {"GOBLIN", MonsterType::GOBLIN},
    {"KNIGHT", MonsterType::KNIGHT},
    {"MUSHROOM", MonsterType::MUSHROOM},
    {"RAT", MonsterType::RAT},
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
  GLACIAL_BURST,
  ICE_LANCE,
  INFERNO_RAY,
  LIGHTNING,
  PYRO_BLAST,
  SOLAR_FLARE,
  THUNDER_SPLASH,
  THUNDER_STRIKE,
  VOID_ERUPTION,
  VOID_FIELD,
  ARCANE_BOLT,
  PSYCHIC_SCREAM,
  SPORE_SHOT,
  SWORD_SPIN,
  BOMB,
  DUMMY,
  LOCKED,
  PROJECTILE_END
};

enum class PlayerSave : uint8_t { EXPERIENCE = 1, COINS, END };
inline static std::unordered_map<std::string, ProjectileType> stringToProjectile = {
    {"POISON_BALL", ProjectileType::POISON_BALL},
    {"FIRE_STRIKE_II", ProjectileType::FIRE_STRIKE_II},
    {"FIRE_STRIKE", ProjectileType::FIRE_STRIKE},
    {"FIRE_BALL", ProjectileType::FIRE_BALL},
    {"BLAST_HAMMER", ProjectileType::BLAST_HAMMER},
    {"SPORE_SHOT", ProjectileType::SPORE_SHOT},
    {"SWORD_SPIN", ProjectileType::SWORD_SPIN},
    {"ENERGY_SPHERE", ProjectileType::ENERGY_SPHERE},
    {"FIRE_SWORD", ProjectileType::FIRE_SWORD},
    {"FROST_NOVA", ProjectileType::FROST_NOVA},
    {"ICE_LANCE", ProjectileType::ICE_LANCE},
    {"ARROW_NORMAL", ProjectileType::ARROW_NORMAL},
    {"INFERNO_RAY", ProjectileType::INFERNO_RAY},
    {"LIGHTNING", ProjectileType::LIGHTNING},
    {"BOMB", ProjectileType::BOMB},
    {"PYRO_BLAST", ProjectileType::PYRO_BLAST},
    {"SOLAR_FLARE", ProjectileType::SOLAR_FLARE},
    {"THUNDER_SPLASH", ProjectileType::THUNDER_SPLASH},
    {"GLACIAL_BURST", ProjectileType::GLACIAL_BURST},
    {"THUNDER_STRIKE", ProjectileType::THUNDER_STRIKE},
    {"VOID_ERUPTION", ProjectileType::VOID_ERUPTION},
    {"PSYCHIC_SCREAM", ProjectileType::PSYCHIC_SCREAM},
    {"LOCKED", ProjectileType::LOCKED},
    {"DUMMY", ProjectileType::DUMMY},
    {"ARCANE_BOLT", ProjectileType::ARCANE_BOLT},
    {"VOID_FIELD", ProjectileType::VOID_FIELD}};

struct ProjectileResources;
struct ProjectileInfo {
  ProjectileResources* res = nullptr;
  float speed = 0;
  float val1 = 0;
  PointT<int16_t> size;
  int16_t lifeSpan = 0;
  ShapeType hitBoxShape = ShapeType::RECT;
  HitType hitType = HitType::ONE_HIT;
  bool illuminated = false;
};

inline static std::unordered_map<ProjectileType, ProjectileInfo> typeToInfo = {};

struct DifficultyMultipliers {
  float healthMult;
  float damageMult;
};

inline static std::unordered_map<Difficulty, DifficultyMultipliers>
    DIFFICULTY_HEALTH_MULT = {{Difficulty::EASY, {0.75F, 0.8F}},
                              {Difficulty::NORMAL, {1.0F, 1.0F}},
                              {Difficulty::HARD, {1.25F, 1.5F}},
                              {Difficulty::TORMENT, {1.75F, 2.0F}}};

inline std::unordered_map<Stat, std::string> statToName = {
    {ARCANE_DMG_P, "Arcane Damage"},
    {DARK_DMG_P, "Dark Damage"},
    {POISON_DMG_P, "Poison Damage"},
    {FIRE_DMG_P, "Fire Damage"},
    {ICE_DMG_P, "Ice Damage"},
    {DOT_LENGTH_P, "DoT Length"},
    {DOT_DAMAGE_P, "DoT Damage"},
    {CRIT_CHANCE, "Crit Chance"},
    {CRIT_DAMAGE_P, "Crit Damage"},
    {STRENGTH, "Strength"},
    {INTELLIGENCE, "Intelligence"},
    {WISDOM, "Wisdom"},
    {VITALITY, "Vitality"},
    {AGILITY, "Agility"},
    {FOCUS, "Focus"},
    {LUCK, "Luck"},
    {BAG_SLOTS, "Bag Slots"},
    {CHARISMA, "Charisma"},
    {ENDURANCE, "Endurance"},
    {BUFF_LENGTH_P, "Buff Length"},
    {HEALTH_REGEN, "Health Regen"},
    {MANA_REGEN, "Mana Regen"},
    {MANA_COST_REDUCTION_P, "Mana Cost Red."},
    {CDR_P, "Cooldown Reduction"},
    {ARMOUR_MULT_P, "Armour Bonus"},
    {MAGIC_FIND, "Magic Find"},
    {MAX_HEALTH, "Max Health"},
    {MAX_MANA, "Max Mana"},
    {WEAPON_DAMAGE, "Weapon Damage"},
    {HEALTH_MULT_P, "Health Bonus"},
    {MANA_MULT_P, "Mana Bonus"},
    {ARMOUR, "Armour"},
    {MAX_SHIELD, "Max Shield"},
    {SPEED_MULT_P, "Speed"},
    {DODGE_CHANCE, "Dodge Chance"},
    {MANA_REGEN_MULT_P, "Mana Regen Mult"},
    {HEALTH_REGEN_MULT_P, "Health Regen Mult"},
    {ARCANE_DMG_WEAKNESS_P, "Arcane Weakness"},
    {DARK_DMG_WEAKNESS_P, "Dark Weakness"},
    {POISON_DMG_WEAKNESS_P, "Poison Weakness"},
    {FIRE_DMG_WEAKNESS_P, "Fire Weakness"},
    {ICE_DMG_WEAKNESS_P, "Ice Weakness"},
    {ARCANE_COST_REDUCTION_P, "Arcane Cost Red"},
    {DARK_COST_REDUCTION_P, "Dark Cost Red"},
    {POISON_COST_REDUCTION_P, "Poison Cost Red"},
    {FIRE_COST_REDUCTION_P, "Fire Cost Red"},
    {ICE_COST_REDUCTION_P, "Ice Cost Red"},
    {DAMAGE_RESISTANCE_P, "Damage Reduction"},
    {XP_MODIFIER_P, "XP Modifier"},
    {GOLD_PICKUP_P, "Coin Find"},
    {FILL8, "Filler Stat 8"},
    {FILL9, "Filler Stat 9"},
    {FILL0, "Filler Stat 0"},
    {FILL10, "Filler Stat 10"},
    {FILL1, "Filler Stat 1"},
    {FILL2, "Filler Stat 2"},
    {FILL3, "Filler Stat 3"},
    {FILL4, "Filler Stat 4"},
    {FILL5, "Filler Stat 5"},
    {FILL11, "Filler Stat 11"},
    {STATS_ENDING, "Stats Ending"}};

inline static std::unordered_map<Zone, std::string> zoneMap = {
    {Zone::Tutorial, "Woodland Edge"},
    {Zone::EtherRealm, "EtherRealm"},
    {Zone::DeadPlains, "DeadPlains"},
    {Zone::GrassLands, "GrassLands"},
    {Zone::City1, "City1"},
    {Zone::Ruin_Dungeon, "Ruin Dungeon"},
    {Zone::Hillcrest, "Hillcrest"},
    {Zone::Hillcrest_Treasure_Cave, "Treasure Cave"},
    {Zone::Hillcrest_Mountain_Cave, "Mountain Cave"},
    {Zone::The_Grove, "The Grove"},
    {Zone::TestRoom, "TestRoom"},
    {Zone::Goblin_Cave, "Goblin Cave"},
    {Zone::Oasis, "Oasis"},
    {Zone::Oasis_Cave, "Forgotten Cave"},
    {Zone::Hillcrest_Hermit_Cave, "Hillcrest Hermit Cave"}};

inline std::unordered_map<Class, std::string> classToString = {
    {Class::BARBARIAN, "Barbarian"}, {Class::BARD, "Bard"},
    {Class::CLERIC, "Cleric"},       {Class::DRUID, "Druid"},
    {Class::FIGHTER, "Fighter"},     {Class::MONK, "Monk"},
    {Class::PALADIN, "Paladin"},     {Class::RANGER, "Ranger"},
    {Class::ROGUE, "Rogue"},         {Class::SORCERER, "Sorcerer"},
    {Class::WARLOCK, "Warlock"},     {Class::WIZARD, "Wizard"}};

inline std::unordered_map<std::string, Stat> attrToStat = {
    {"FOC", FOCUS},  {"END", ENDURANCE}, {"STR", STRENGTH}, {"INT", INTELLIGENCE},
    {"WIS", WISDOM}, {"CHA", CHARISMA},  {"VIT", VITALITY}, {"AGI", AGILITY},
    {"LUC", LUCK},   {"BAG", BAG_SLOTS}};
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
    {"Hillcrest_Hermit_Cave", Zone::Hillcrest_Hermit_Cave},
    {"Oasis", Zone::Oasis},
    {"Oasis_Cave", Zone::Oasis_Cave}};

inline static std::unordered_map<std::string, ItemSetNum> stringToItemSet = {
    {"ROMEO_JULIET", ItemSetNum::ROMEO_JULIET},
    {"DEVILS_AVATAR", ItemSetNum::DEVILS_AVATAR},
};

inline Difficulty GAME_DIFFICULTY = Difficulty::NORMAL;
#endif  //MAGE_QUEST_SRC_ENUMS_ENUMS_H_