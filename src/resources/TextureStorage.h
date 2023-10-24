#ifndef MAGEQUEST_SRC_SYSTEM_TEXTURESTORAGE_H_
#define MAGEQUEST_SRC_SYSTEM_TEXTURESTORAGE_H_

namespace textures {
inline Texture EMPTY_TEXTURE = Texture();
inline MonsterResource PLAYER_RESOURCE;

namespace npc {
inline MonsterResource OLD_MAN;
inline MonsterResource ARIA;
inline MonsterResource NIETZSCHE;
inline MonsterResource realmKeeper;
inline MonsterResource MARLA;
}  // namespace npc

namespace monsters {
inline MonsterResource SKELETON_WARRIOR;
inline MonsterResource SKELETON_SPEAR;
inline MonsterResource SKELETON_SHIELD;
inline MonsterResource SKELETON_ARCHER;
inline MonsterResource BOSS_DEATH_BRINGER;
inline MonsterResource BOSS_KNIGHT;
inline MonsterResource BOSS_SLIME;
inline MonsterResource GOBLIN;
inline MonsterResource KNIGHT;
inline MonsterResource MUSHROOM;
inline MonsterResource SNAKE;
inline MonsterResource WOLF;
inline MonsterResource BLOOD_HOUND;
inline MonsterResource GHOST;
}  // namespace monsters

namespace projectile {
inline ProjectileResources BLAST_HAMMER;
inline ProjectileResources ENERGY_SPHERE;
inline ProjectileResources ETHER_PORTAL;
inline ProjectileResources FIRE_BURST;
inline ProjectileResources FIRE_EXPLOSION;
inline ProjectileResources FIRE_SWORD;
inline ProjectileResources FROST_NOVA;
inline ProjectileResources GATE_1;
inline ProjectileResources ICE_LANCE;
inline ProjectileResources INFERNO_RAY;
inline ProjectileResources LIGHTNING_STRIKE;
inline ProjectileResources POWER_SURGE;
inline ProjectileResources PYRO_BLAST;
inline ProjectileResources SOLAR_FLARE;
inline ProjectileResources THUNDER_SPLASH;
inline ProjectileResources THUNDER_STRIKE;
inline ProjectileResources VOID_ERUPTION;
inline ProjectileResources VOID_FIELD;
inline ProjectileResources DUMMY;
inline ProjectileResources PSYCHIC_SCREAM;
inline ProjectileResources ARROW_NORMAL;

}  // namespace projectile

namespace misc {
inline static Texture blueGlow;
inline static Texture redGlow;
inline static Texture purpleGlow;
}  // namespace misc
namespace ui {
inline Texture STATUS_BAR;
inline Texture CROSS_HAIR_AIM;
inline Texture CROSS_HAIR_SELECT;
inline Texture ETHER_MARK;
inline Texture HEALTH_BAR;
inline Texture KEY_MAP;
inline static Texture bossbar;
inline static Texture toolTip;
inline static Texture buttonNormal;
inline static Texture buttonHovered;
inline static Texture buttonPressed;

inline static Texture buttonExpand;
inline static Texture buttonExpandHovered;
inline static Texture buttonCollapse;
inline static Texture buttonCollapseHovered;
inline static Texture dialogContinue;
inline static Texture dialogContinue2;
inline static Texture dialogContinue3;

inline static Texture notificationWindow;
inline static Texture spendButtonNormal;
inline static Texture spendButtonHovered;
inline static Texture spendButtonPressed;

namespace questpanel {
inline static Texture questBox;
inline static Texture questBoxHovered;
inline static Texture questBoxPressed;

inline static Texture choiceBox;
inline static Texture choiceBoxHovered;
}  // namespace questpanel

namespace skillbar {
inline static Texture bag;
inline static Texture bag_hovered;
inline static Texture bag_pressed;

inline static Texture abilities;
inline static Texture abilities_hovered;
inline static Texture abilities_pressed;

inline static Texture character;
inline static Texture character_hovered;
inline static Texture character_pressed;

inline static Texture map;
inline static Texture map_hovered;
inline static Texture map_pressed;

inline static Texture setting;
inline static Texture setting_hovered;
inline static Texture setting_pressed;

inline static Texture skilltree;
inline static Texture skilltree_hovered;
inline static Texture skilltree_pressed;

inline static Texture tooltip;
inline static Texture mouseleft;
inline static Texture mouseright;
inline static Texture skillbar;
inline static Texture xpbar;
inline static Texture skillRange;

namespace icons {
inline static Texture iceLance;
inline static Texture infernoRay;
inline static Texture inspiration;
inline static Texture lightning;
inline static Texture powerSurge;
inline static Texture pyroBlast;
inline static Texture regenAura;
inline static Texture ring_salvo;
inline static Texture slimeBall;
inline static Texture solarFlare;
inline static Texture thunderSplash;
inline static Texture thunderStrike;
inline static Texture voidEruption;
inline static Texture voidField;
inline static Texture blastHammer;
inline static Texture energy_sphere;
inline static Texture fireBurst;
inline static Texture fireSword;
inline static Texture frostNova;
inline static Texture locked;
}  // namespace icons
}  // namespace skillbar

}  // namespace ui

inline std::vector<Texture> DISCORD_ANIMATION;
inline std::vector<Texture> GITHUB_ANIMATION;

inline Texture amulet;
inline Texture bag;
inline Texture boots;
inline Texture chest;
inline Texture helm;
inline Texture offhand;
inline Texture pants;
inline Texture relic;
inline Texture ring;
inline Texture weapon;

//TALENTS
inline Texture TALENT_BACKGROUND_TILE;
inline Texture CONNECTION_GREEN;
inline Texture CONNECTION_ORANGE;
inline Texture CONNECTION_RED;
inline Texture NODE_GREEN;
inline Texture NODE_ORANGE;
inline Texture NODE_PURPLE;
inline Texture TALENT_BIG;
inline Texture TALENT_BIG_GREEN;
inline Texture TALENT_BIG_PURPLE;
inline Texture TALENT_MID;
inline Texture TALENT_MID_GREEN;
inline Texture TALENT_MID_PURPLE;
inline Texture TALENT_NODE;

}  // namespace textures
#endif  //MAGEQUEST_SRC_SYSTEM_TEXTURESTORAGE_H_
