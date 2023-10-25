#ifndef MAGEQUEST_SRC_GAMEPLAY_HANDLERS_ITEMDROPHANDLER_H_
#define MAGEQUEST_SRC_GAMEPLAY_HANDLERS_ITEMDROPHANDLER_H_

namespace ItemDropHandler {
inline static Item* GetNewItem(int id, ItemType type, int quality, int level) {
  for (const auto& item : ITEMS) {
    if (item.id == id && item.type == type) {
      return new Item(item.id, item.name, item.rarity, item.type, item.description,
                      item.texture, quality, level);
    }
  }
  return nullptr;
}
inline static const Item* GetRandomItemPtr() noexcept {
  const Item* item;
  while ((item = &ITEMS[RANGE_EXISTING_ITEMS(RNG_ENGINE)]) &&
         item->type == ItemType::MISC) {}
  return item;
}
inline static const Item* GetRandomItemPtr(ItemRarity rarity) noexcept {
  const Item* item;
  while ((item = GetRandomItemPtr()) && item->rarity != rarity) {}
  return item;
}
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
inline static Item* CreateNewScaledItem(const Item* ptr, int quality,
                                        int level) noexcept {
  Item* newItem = new Item(ptr, quality, level);
  float levelMult = std::sqrt(level);
  float rarityMult = std::min(4, (int)ptr->rarity);
  float qualityMult = quality/100.0F;
  if(qualityMult == 1) {
    qualityMult = 1.1F;
  }
  for (float& effect : newItem->effects) {
    effect *= levelMult * rarityMult * qualityMult;
  }
  return newItem;
}
inline static void RollForItemDrop(float x, float y, int level) noexcept {
  float luck = PLAYER_STATS.effects[LUCK] / (std::sqrt(level) * std::sqrt(level));

  if (RANGE_100(RNG_ENGINE) < 85 - luck) return;

  int quality = RANGE_ItemQuality(RNG_ENGINE) + luck;
  quality = std::min(100, quality);

  auto rarity = RollRarity(luck);

  Item* dropItem = CreateNewScaledItem(GetRandomItemPtr(rarity), quality, level);

  WORLD_OBJECTS.push_back(new DroppedItem({x, y}, dropItem));
}

}  // namespace ItemDropHandler
#endif  //MAGEQUEST_SRC_GAMEPLAY_HANDLERS_ITEMDROPHANDLER_H_
