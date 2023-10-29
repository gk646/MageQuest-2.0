#ifndef MAGEQUEST_SRC_GAMEPLAY_HANDLERS_ITEMDROPHANDLER_H_
#define MAGEQUEST_SRC_GAMEPLAY_HANDLERS_ITEMDROPHANDLER_H_

Item* Item::FindBaseItem(int id, ItemType type, int quality, int level) {
  for (const auto& item : ITEMS) {
    if (item.id == id && item.type == type) {
      auto clone = new Item(item);
      clone->quality = quality;
      clone->level = level;
      return clone;
    }
  }
  return nullptr;
}
namespace ItemDropHandler {
//Safety namespace for handling base items (non-scaled)
namespace BaseItem {
//Get random non MISC item
inline static const Item* GetRandomItemPtr() noexcept {
  const Item* item;
  while ((item = &ITEMS[RANGE_EXISTING_ITEMS(RNG_ENGINE)]) &&
         item->type == ItemType::MISC) {}
  return item;
}
//Get random non MISC item with given type
inline static const Item* GetRandomItemPtr(ItemType type) noexcept {
  const Item* item;
  while ((item = &ITEMS[RANGE_EXISTING_ITEMS(RNG_ENGINE)]) &&
             item->type == ItemType::MISC ||
         item->type != type) {}

  return item;
}
//Get random non MISC item with given rarity
inline static const Item* GetRandomItemPtr(ItemRarity rarity) noexcept {
  const Item* item;
  while ((item = GetRandomItemPtr()) && item->rarity != rarity) {}
  return item;
}
}  // namespace BaseItem
//Get the rarity for an item drop
inline static ItemRarity RollRarity(float luck) noexcept {
  float num = RANGE_100_FLOAT(RNG_ENGINE) + luck;
  if (num < 60) {
    return ItemRarity::NORMAL;
  } else if (num < 90) {
    return ItemRarity::RARE;
  } else if (num < 97) {
    return ItemRarity::EPIC;
  } else if (num < 99.5) {
    return ItemRarity::LEGENDARY;
  } else {
    return ItemRarity::SET_ITEM;
  }
}
//Clones the item and scales its base values according to scaling rules
inline static Item* CreateNewScaledItem(const Item* ptr, int quality,
                                        int level) noexcept {
  Item* newItem = new Item(*ptr);
  newItem->quality = quality;
  newItem->level = level;
  newItem->durability = (uint8_t)(50 + RANGE_01(RNG_ENGINE) * 50);

  float levelMult = std::sqrt(level);
  float rarityMult = std::min(3, (int)ptr->rarity);
  float qualityMult = quality / 100.0F;
  if (qualityMult == 1) {
    qualityMult = 1.1F;
  }

  for (uint_fast32_t i = 0; i < BAG_SLOTS; i++) {
    if (newItem->effects[i] < 0) {
      newItem->effects[i] = (newItem->effects[i] - rarityMult) * levelMult * qualityMult;
    } else {
      newItem->effects[i] = (newItem->effects[i] + rarityMult) * levelMult * qualityMult;
    }
  }

  return newItem;
}
//Adds the item to the global ITEM array
inline static void DropItem(float x, float y, Item* item) noexcept;
//Returns a ptr to a new random scaled item
inline static Item* GetRandomScaledItem(int level) noexcept {
  float luck = PLAYER_STATS.effects[LUCK] / (std::sqrt(level) * std::sqrt(level));
  int quality = RANGE_ItemQuality(RNG_ENGINE) + luck;
  quality = std::min(100, quality);

  auto rarity = RollRarity(luck);

  return CreateNewScaledItem(BaseItem::GetRandomItemPtr(rarity), quality, level);
}
//Returns a ptr to a new random scaled item with a minimum rarity of "minRarity"
inline static Item* GetRandomScaledItemRarityGoal(ItemRarity minRarity, int level) {
  float luck = PLAYER_STATS.effects[LUCK] / (std::sqrt(level) * std::sqrt(level));
  int quality = RANGE_ItemQuality(RNG_ENGINE) + luck;
  quality = std::min(100, quality);

  ItemRarity rarity;
  while ((rarity = RollRarity(luck)) < minRarity) {}

  return CreateNewScaledItem(BaseItem::GetRandomItemPtr(rarity), quality, level);
}
//Usability method to roll for an item drop with a single call
inline static void RollForItemDrop(float x, float y, int level) noexcept {
  float luck = PLAYER_STATS.effects[LUCK] / (std::sqrt(level) * std::sqrt(level));
  if (RANGE_100(RNG_ENGINE) < 85 - luck) return;
  DropItem(x, y, GetRandomScaledItem(level));
}
}  // namespace ItemDropHandler
#endif  //MAGEQUEST_SRC_GAMEPLAY_HANDLERS_ITEMDROPHANDLER_H_
