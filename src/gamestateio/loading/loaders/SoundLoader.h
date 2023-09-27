#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
struct SoundLoader{

  static void load(){
    sound::intro = LoadSound((ASSET_PATH+"sound/music/intro.wav").c_str());

    sound::buy = LoadSound((ASSET_PATH+"sound/effects/inventory/buy.wav").c_str());
    sound::close_inventory = LoadSound((ASSET_PATH+"sound/effects/inventory/closeInventory.wav").c_str());
    sound::equip = LoadSound((ASSET_PATH+"sound/effects/inventory/equip.wav").c_str());
    sound::open_inventory = LoadSound((ASSET_PATH+"sound/effects/inventory/openInventory.wav").c_str());
    sound::pickup_gold = LoadSound((ASSET_PATH+"sound/effects/inventory/pickupGold.wav").c_str());
    sound::read_book = LoadSound((ASSET_PATH+"sound/effects/inventory/readBook.wav").c_str());
    sound::sell = LoadSound((ASSET_PATH+"sound/effects/inventory/sell.wav").c_str());
    sound::zipTabs = LoadSound((ASSET_PATH+"sound/effects/inventory/zipTabs.wav").c_str());



    sound::completeQuest = LoadSound((ASSET_PATH+"sound/effects/quest/complete.wav").c_str());
    sound::majorObjective = LoadSound((ASSET_PATH+"sound/effects/quest/finish_objective.wav").c_str());
    sound::interact = LoadSound((ASSET_PATH+"sound/effects/quest/interact.wav").c_str());
    sound::speak = LoadSound((ASSET_PATH+"sound/effects/quest/speak.wav").c_str());

  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
