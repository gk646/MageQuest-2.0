#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEM_H_

struct Item {
  static constexpr int tltip_x = 260;
  static constexpr int tltip_y = 350;
  static char text_buffer[10];
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
  Item(int id, std::string name, ItemRarity rarity, ItemType type,
       std::string description, const Texture& texture, int quality, int level)
      : id(id),
        name(std::move(name)),
        rarity(rarity),
        type(type),
        description(std::move(description)),
        texture(texture),
        quality(quality),
        level(level) {}
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
        texture(other.texture),
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
    DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, rect,
                   {0, 0}, 0, WHITE);
  }
  void draw_tooltip(const Rectangle& rect) const noexcept {
    float width = tltip_x * UI_SCALE;
    float height = tltip_y * UI_SCALE;
    float startX = rect.x - width;
    float startY = rect.y - height;

    DrawRectangleRounded({startX, startY, width, height}, 0.1, 25, Colors::LightGrey);
    DrawRectangleRoundedLines({startX, startY, width, height}, 0.1, 25, 2,
                              rarity_to_color[rarity]);

    //ilvl
    snprintf(text_buffer, 10, "ilvl:%d", level);
    DrawTextEx(MINECRAFT_ITALIC, text_buffer,
               {startX + 3 * UI_SCALE, startY + 3 * UI_SCALE}, 16 * UI_SCALE, 1,
               Colors::darkBackground);

    //quality
    snprintf(text_buffer, 10, "%d%%", quality);
    if (quality == 100) [[unlikely]] {
      DrawTextEx(MINECRAFT_REGULAR, text_buffer,
                 {startX + width - 36 * UI_SCALE, startY + 3 * UI_SCALE}, 16 * UI_SCALE,
                 1, get_quality_color());
    } else {
      DrawTextEx(MINECRAFT_REGULAR, text_buffer,
                 {startX + width - 32 * UI_SCALE, startY + 3 * UI_SCALE}, 16 * UI_SCALE,
                 1, get_quality_color());
    }

    //name
    DrawTextEx(MINECRAFT_BOLD, name.c_str(),
               {startX + 5 * UI_SCALE, startY + 30 * UI_SCALE}, 21 * UI_SCALE, 1,
               rarity_to_color[rarity]);

    //attributes
    int off_setX = 6 * UI_SCALE;
    int off_sety = 70 * UI_SCALE;
    int v_space = 15 * UI_SCALE;
    int h_space = 90 * UI_SCALE;
    int font_size = 16 * UI_SCALE;

    snprintf(text_buffer, 10, "INT: %d", (int)effects[INTELLIGENCE]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer, {startX + off_setX, startY + off_sety},
               font_size, 1, Colors::darkBackground);

    snprintf(text_buffer, 10, "WIS: %d", (int)effects[WISDOM]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX, startY + off_sety + v_space}, font_size, 1,
               Colors::darkBackground);
    snprintf(text_buffer, 10, "FOC: %d", (int)effects[FOCUS]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX, startY + off_sety + v_space * 2}, font_size, 1,
               Colors::darkBackground);

    snprintf(text_buffer, 10, "VIT: %d", (int)effects[VITALITY]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX + h_space, startY + off_sety}, font_size, 1,
               Colors::darkBackground);
    snprintf(text_buffer, 10, "AGI: %d", (int)effects[AGILITY]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX + h_space, startY + off_sety + v_space}, font_size, 1,
               Colors::darkBackground);
    snprintf(text_buffer, 10, "END: %d", (int)effects[ENDURANCE]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX + h_space, startY + off_sety + v_space * 2}, font_size,
               1, Colors::darkBackground);

    snprintf(text_buffer, 10, "STR: %d", (int)effects[STRENGTH]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX + h_space * 2, startY + +off_sety}, font_size, 1,
               Colors::darkBackground);
    snprintf(text_buffer, 10, "CHA: %d", (int)effects[CHARISMA]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX + h_space * 2, startY + off_sety + v_space}, font_size,
               1, Colors::darkBackground);
    snprintf(text_buffer, 10, "LUC: %d", (int)effects[LUCK]);
    DrawTextEx(MINECRAFT_REGULAR, text_buffer,
               {startX + off_setX + h_space * 2, startY + off_sety + v_space * 2},
               font_size, 1, Colors::darkBackground);

    //effets
    off_sety = 140 * UI_SCALE;
    for (uint_fast32_t i = 9; i < WEAPON_DAMAGE; i++) {
      if (effects[i] != 0) {
        snprintf(text_buffer, 10, "%+0.2f", effects[i]*100);
        std::string displayText =
            std::string(text_buffer) + " " + statToName[Stat(i)];
        DrawTextEx(MINECRAFT_REGULAR, displayText.c_str(),
                   {startX + off_setX, startY + off_sety}, font_size, 1,
                   Colors::darkBackground);
        off_sety+=font_size;
      }
    }
    for (uint_fast32_t i = WEAPON_DAMAGE; i < STATS_ENDING; i++) {
      if (effects[i] != 0) {
        snprintf(text_buffer, 10, "%+0.2f", effects[i]);
        std::string displayText =
            std::string(text_buffer) + " " + statToName[Stat(i)];
        DrawTextEx(MINECRAFT_REGULAR, displayText.c_str(),
                   {startX + off_setX, startY + off_sety}, font_size, 1,
                   Colors::darkBackground);
        off_sety+=font_size;
      }
    }

    //description
    off_sety = 180 * UI_SCALE;
    std::vector<std::string> lines = WrapText(description, width);
    for (const auto& line : lines) {
      DrawTextEx(MINECRAFT_ITALIC, line.c_str(), {startX + off_setX, startY + off_sety}, 15*UI_SCALE, 0.8F,
                 Colors::darkBackground);
      off_sety += 14;
    }
  }
  inline Color get_quality_color() const noexcept {
    if (quality < 90) [[likely]] {
      return Colors::NormalGrey;
    } else if (quality == 100) {
      return Colors::highQuality;
    } else {
      return Colors::mediumQuality;
    }
  }
  static std::vector<std::string> WrapText(const std::string& str, int width) {
    std::vector<std::string> lines;
    std::istringstream stream(str);
    std::string word;

    std::string currentLine;
    while (stream >> word) {
      if (GetTextWidth((currentLine + word).c_str())+5 <= width) {
        if (!currentLine.empty()) {
          currentLine += " ";
        }
        currentLine += word;
      } else {
        lines.push_back(currentLine);
        currentLine = word;
      }
    }

    if (!currentLine.empty()) {
      lines.push_back(currentLine);
    }

    return lines;
  }
  inline static void scale(int quality, int level) {}
  Item* scale_clone(int new_level, int new_quality) const noexcept {
    auto new_item = new Item(*this);
    return new_item;
  }
};
char Item::text_buffer[10];
inline bool WINDOW_FOCUSED = false;
inline Item* DRAGGED_ITEM;
#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
