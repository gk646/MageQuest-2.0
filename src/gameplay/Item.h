#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEM_H_

struct Item {
  float effects[STATS_ENDING] = {0};
  int quality = 70;
  int durability = 100;
  int level = 1;
  int id = 0;
  std::string name;
  std::string description;
  Texture texture;
  ItemRarity rarity = ItemRarity::NORMAL;
  ItemType type = ItemType::RING;
  Item(int id, std::string name, ItemRarity rarity, ItemType type,
       std::string description, const Texture& texture)
      : id(id),
        name(std::move(name)),
        rarity(rarity),
        type(type),
        description(std::move(description)),
        texture(texture) {}
  Item(const Item* base_item, int quality, int level, int durability = 100)
      : Item(*base_item) {
    this->quality = quality;
    this->level = level;
    this->durability = durability;
    scale(quality, level);
  }
  Item(const Item& other)
      : id(other.id),
        quality(other.quality),
        durability(other.durability),
        level(other.level),
        name(other.name),
        description(other.description),
        texture(other.texture),
        rarity(other.rarity),
        type(other.type) {
    std::copy(std::begin(other.effects), std::end(other.effects), std::begin(effects));
  }
  Item& operator=(const Item& other) {
    if (this == &other) {
      return *this;
    }
    id = other.id;
    quality = other.quality;
    durability = other.durability;
    level = other.level;
    name = other.name;
    description = other.description;
    texture = other.texture;
    rarity = other.rarity;
    type = other.type;
    std::copy(std::begin(other.effects), std::end(other.effects), std::begin(effects));
    return *this;
  }
  Item(Item&& other) noexcept
      : id(other.id),
        quality(other.quality),
        durability(other.durability),
        level(other.level),
        name(std::move(other.name)),
        description(std::move(other.description)),
        texture(std::move(other.texture)),
        rarity(other.rarity),
        type(other.type) {
    std::move(std::begin(other.effects), std::end(other.effects), std::begin(effects));
  }
  Item& operator=(Item&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    id = other.id;
    quality = other.quality;
    durability = other.durability;
    level = other.level;
    name = std::move(other.name);
    description = std::move(other.description);
    texture = std::move(other.texture);
    rarity = other.rarity;
    type = other.type;
    std::move(std::begin(other.effects), std::end(other.effects), std::begin(effects));
    return *this;
  }
  void draw(const Rectangle& rect) const noexcept {
    DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height},
                   rect, {0, 0}, 0, WHITE);
  }
  void draw_tooltip(const Rectangle& rect) const noexcept {
    int startX = rect.x - 100;
    int startY = rect.y - 100;
    DrawRectangle(startX, startY, 100, 200, Colors::LightGrey);
    DrawRectangleLinesEx({(float)startX, (float)startY, 100, 200}, 2,
                         Colors::darkBackground);

    DrawTextEx(MINECRAFT_BOLD, name.c_str(),
               {(float)startX + 5, (float)startY + 5}, 16, 1,
               Colors::darkBackground);
  }
  inline static void scale(int quality, int level) {}
  Item scale_clone(int new_level, int new_quality) const noexcept {
    auto new_item = Item(*this);
    return new_item;
  }
};
inline bool WINDOW_FOCUSED = false;
inline Item* DRAGGED_ITEM;
#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
