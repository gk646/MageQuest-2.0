#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEM_H_

struct Item {
  int quality;
  int durability;
  int level;
  std::string name;
  Texture texture;
  ItemRarity rarity;
  ItemType item_type;
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
