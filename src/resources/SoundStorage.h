#ifndef MAGEQUEST_SRC_SYSTEM_SOUNDSTORAGE_H_
#define MAGEQUEST_SRC_SYSTEM_SOUNDSTORAGE_H_

#include "../world/elements/MusicStreamer.h"
namespace sound {
inline static Sound EMPTY_SOUND;

namespace environment {
inline static Sound spikes;
}
namespace interactable {
inline static Sound basicChest;
inline static Sound normalChest;
inline static Sound epicChest;
inline static Sound lever;
}  // namespace interactable

namespace music {
inline static Music firePlace;
inline static Music lava;
inline static Music hillcrestVillage;
inline static Playlist waterAmbience{};
inline static Playlist townAmbience{};
inline static Playlist forestAmbience{};
inline static Playlist forestNightAmbience{};
inline static Playlist dungeonAmbience{};
inline static Playlist caveAmbience{};
inline static Playlist bossMusic{};
inline static Music moonLightSonata;
}  // namespace music

namespace talents {
inline static Sound spendTalentPoint;
}
namespace player {
inline static std::vector<Sound> grassWalk;
inline static Sound abilityCast;
}  // namespace player
inline Sound intro;

inline Sound menu_back;
inline Sound menu_switch;

//inventory
inline Sound buy;
inline Sound closeBags;
inline Sound openBags;
inline Sound closeInventory;
inline Sound openInventory;
inline Sound equip;
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
inline static Sound glacialBurst;
inline static Sound voidField;
inline static Sound voidEruption;
}  // namespace sound
#endif  //MAGEQUEST_SRC_SYSTEM_SOUNDSTORAGE_H_
