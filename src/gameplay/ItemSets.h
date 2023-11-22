#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEMSETS_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEMSETS_H_

struct Item;
struct ItemIdentifier {
  int8_t id = -1;
  ItemType type = ItemType::EMPTY;
};

struct ItemSet {
  std::string title{};
  std::string descriptions[3];
  std::array<ItemIdentifier, 4> items{};
  int16_t toolTipHeight = 0;
  int8_t thresholds[3] = {0};
  int8_t bonusCount = 0;
  inline static bool IsPartOfItemSet(Item* item, const ItemSet& set) noexcept;
  inline static int CountEquippedSetItemsOfSet(const ItemSet& set) noexcept;
};

inline static std::array<ItemSet, (int)ItemSetNum::END> ITEM_SETS;

#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEMSETS_H_
