#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
struct SoundLoader {

  static void Load() {
    LoadSoundIntoVector("sound/effects/player/walkSounds/grass/",
                        sound::player::grassWalk, 0.3F);

    sound::player::abilityCast =
        LoadSound((ASSET_PATH + "sound/effects/player/cast.wav").c_str());
    sound::talents::spendTalentPoint =
        LoadSound((ASSET_PATH + "sound/effects/levelup/getTalent.wav").c_str());
    sound::interactable::basicChest =
        LoadSound((ASSET_PATH + "sound/effects/interactable/chestOpen.wav").c_str());
    sound::interactable::normalChest =
        LoadSound((ASSET_PATH + "sound/effects/interactable/chestOpen.wav").c_str());
    sound::interactable::epicChest =
        LoadSound((ASSET_PATH + "sound/effects/interactable/epicChestOpen.wav").c_str());
    sound::interactable::lever =
        LoadSound((ASSET_PATH + "sound/effects/interactable/lever.wav").c_str());

    sound::environment::spikes =
        LoadSound((ASSET_PATH + "sound/effects/environment/spike.wav").c_str());

    //Projectile SOUNDS
    sound::energySphere = LoadSound(
        (ASSET_PATH + "sound/effects/projectiles/energySphere/fullsound.wav").c_str());
    sound::powerSurgeUp =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/powerSurge/0.wav").c_str());
    sound::powerSurgeDown =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/powerSurge/1.wav").c_str());

    sound::blastHammer =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/blastHammer/0.wav").c_str());
    sound::fireBurst =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/fireBurst/0.wav").c_str());
    sound::fireSword =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/fireSword/0.wav").c_str());
    sound::frostNova =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/frostNova/0.wav").c_str());
    sound::iceLance =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/iceLance/0.wav").c_str());
    sound::infernoRay =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/infernoRay/0.wav").c_str());
    sound::lightning =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/lightning/0.wav").c_str());
    sound::pyroBlast =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/pyroBlast/0.wav").c_str());
    sound::solarFlare =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/solarFlare/0.wav").c_str());
    sound::thunderSplash =
        LoadSound((ASSET_PATH + "sound/effects/projectiles/thunderSplash/0.wav").c_str());
    sound::thunderStrike =        LoadSound((ASSET_PATH + "sound/effects/projectiles/thunderStrike/0.wav").c_str());
    sound::glacialBurst =        LoadSound((ASSET_PATH + "sound/effects/projectiles/glacialBurst/0.wav").c_str());
    sound::voidEruption =        LoadSound((ASSET_PATH + "sound/effects/projectiles/voidEruption/0.wav").c_str());
    sound::voidField =        LoadSound((ASSET_PATH + "sound/effects/projectiles/voidField/0.wav").c_str());

    //UI SOUNDS
    sound::intro = LoadSound((ASSET_PATH + "sound/music/intro.wav").c_str());
    sound::buy = LoadSound((ASSET_PATH + "sound/effects/inventory/buy.wav").c_str());
    sound::equip = LoadSound((ASSET_PATH + "sound/effects/inventory/equip.wav").c_str());
    sound::closeBags =
        LoadSound((ASSET_PATH + "sound/effects/inventory/closeBags.wav").c_str());
    sound::openBags =
        LoadSound((ASSET_PATH + "sound/effects/inventory/openBags.wav").c_str());
    sound::openInventory =
        LoadSound((ASSET_PATH + "sound/effects/inventory/openInventory.wav").c_str());
    sound::closeInventory =
        LoadSound((ASSET_PATH + "sound/effects/inventory/closeInventory.wav").c_str());
    sound::equip = LoadSound((ASSET_PATH + "sound/effects/inventory/equip.wav").c_str());
    sound::pickup_gold =
        LoadSound((ASSET_PATH + "sound/effects/inventory/pickupGold.wav").c_str());
    sound::read_book =
        LoadSound((ASSET_PATH + "sound/effects/inventory/readBook.wav").c_str());
    sound::sell = LoadSound((ASSET_PATH + "sound/effects/inventory/sell.wav").c_str());
    sound::zipTabs =
        LoadSound((ASSET_PATH + "sound/effects/inventory/zipTabs.wav").c_str());

    sound::completeQuest =
        LoadSound((ASSET_PATH + "sound/effects/quest/complete.wav").c_str());
    sound::majorObjective =
        LoadSound((ASSET_PATH + "sound/effects/quest/finish_objective.wav").c_str());
    sound::interact =
        LoadSound((ASSET_PATH + "sound/effects/quest/interact.wav").c_str());
    sound::speak = LoadSound((ASSET_PATH + "sound/effects/quest/speak.wav").c_str());

    sound::menu_switch =
        LoadSound((ASSET_PATH + "sound/effects/ui/menu_switch.wav").c_str());
    sound::menu_back = LoadSound((ASSET_PATH + "sound/effects/ui/menu_back.wav").c_str());

    sound::EMPTY_SOUND = LoadSound((ASSET_PATH + "sound/effects/empty.wav").c_str());
    SetVolumes();
  }

  static void SetVolumes() noexcept {
    SetSoundVolume(sound::menu_back, 1);

    SetSoundVolume(sound::energySphere, 0.4F);
    SetSoundVolume(sound::fireBurst, 0.7F);

    SetSoundVolume(sound::openBags, 0.8F);
    SetSoundVolume(sound::closeBags, 0.8F);

    SetSoundVolume(sound::openInventory, 0.3F);
    SetSoundVolume(sound::closeInventory, 0.3F);

    SetSoundVolume(sound::player::abilityCast, 0.7F);





  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
