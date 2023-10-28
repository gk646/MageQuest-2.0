#ifndef MAGEQUEST_SRC_SYSTEM_SOUNDSTORAGE_H_
#define MAGEQUEST_SRC_SYSTEM_SOUNDSTORAGE_H_

namespace sound {
inline static Sound EMPTY_SOUND;

namespace environment{
inline static Sound spikes;
}
namespace interactable{
inline static Sound basicChest;
inline static Sound normalChest;
inline static Sound epicChest;
inline static Sound lever;
}
namespace music {
inline static Music firePlace;
inline static Music lava;
inline static Music hillcrestVillage;
inline static std::vector<Music> waterAmbience;
inline static std::vector<Music> townAmbience;
inline static std::vector<Music> forestAmbience;
inline static std::vector<Music> dungeonAmbience;
inline static std::vector<Music> bossMusic;
inline static Music moonLightSonata;
}  // namespace music

inline Sound intro;

inline Sound menu_back;
inline Sound menu_switch;

//inventory
inline Sound buy;
inline Sound close_inventory;
inline Sound equip;
inline Sound open_inventory;
inline Sound pickup_gold;
inline Sound read_book;
inline Sound sell;
inline Sound zipTabs;

//quests
inline static Sound majorObjective;
inline static Sound completeQuest;
inline static Sound interact;
inline static Sound speak;
/* |-----------------------------------------------------|
 * |                    PROJECTILES                      |
 * |-----------------------------------------------------|
 */

inline static Sound blastHammer;
inline static Sound energySphere;
inline static Sound energySphere_hit;
inline static Sound fireBurst;
inline static Sound fireSword;
inline static Sound frostNova;
inline static Sound iceLance;
inline static Sound infernoRay;
inline static Sound lightning;
inline static Sound powerSurgeUp;
inline static Sound powerSurgeDown;
inline static Sound pyroBlast;
inline static Sound solarFlare;
inline static Sound thunderSplash;
inline static Sound thunderStrike;
}  // namespace sound
#endif  //MAGEQUEST_SRC_SYSTEM_SOUNDSTORAGE_H_
