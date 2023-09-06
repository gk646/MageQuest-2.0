#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEM_H_

struct Item {
  float effects[STATS_ENDING] = {0};
  int quality = 70;
  int durability = 100;
  int level = 1;
  std::string name = "Hello";
  std::string description{};
  Texture texture{};
  ItemRarity rarity = ItemRarity::NORMAL;
  ItemType type = ItemType::HEAD;
  Item() { effects[MAX_HEALTH] = 10; }
  Item(std::string name, std::string description, ItemRarity rarity,
       ItemType type)
      : name(std::move(name)),
        description(std::move(description)),
        rarity(rarity),
        type(type) {}
  Item(const Item& other)
      : quality(other.quality),
        durability(other.durability),
        level(other.level),
        name(other.name),
        description(other.description),
        texture(other.texture),
        rarity(other.rarity),
        type(other.type) {}
  Item& operator=(const Item& other) {
    if (this == &other) {
      return *this;
    }
    quality = other.quality;
    durability = other.durability;
    level = other.level;
    name = other.name;
    description = other.description;
    texture = other.texture;
    rarity = other.rarity;
    type = other.type;
    return *this;
  }
  Item(Item&& other) noexcept
      : quality(other.quality),
        durability(other.durability),
        level(other.level),
        name(std::move(other.name)),
        description(std::move(other.description)),
        texture(other.texture),
        rarity(other.rarity),
        type(other.type) {}
  Item& operator=(Item&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    quality = std::move(other.quality);
    durability = std::move(other.durability);
    level = std::move(other.level);
    name = std::move(other.name);
    description = std::move(other.description);
    texture = std::move(other.texture);
    rarity = std::move(other.rarity);
    type = std::move(other.type);
    return *this;
  }
  void draw(const Rectangle& rect) const noexcept {
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLUE);
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
  Item scale_clone(int new_level, int new_quality) const noexcept {
    auto new_item = Item(*this);
    return new_item;
  }
};
inline bool WINDOW_FOCUSED = false;
inline Item* DRAGGED_ITEM;
#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
