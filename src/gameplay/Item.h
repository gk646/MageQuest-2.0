#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEM_H_

struct Item {
  static constexpr int tooltip_x = 260;
  static constexpr int tooltip_y = 350;
  static char text_buffer[10];
  uint8_t quality = 70;
  uint8_t durability = 100;
  uint8_t level = 1;
  uint8_t id = 0;
  Texture texture;
  float effects[STATS_ENDING] = {0};
  ItemRarity rarity = ItemRarity::NORMAL;
  ItemType type = ItemType::RING;
  std::string name;
  std::string description;
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
    texture = other.texture;
    rarity = other.rarity;
    type = other.type;
    std::move(std::begin(other.effects), std::end(other.effects), std::begin(effects));
    return *this;
  }
  void Draw(const RectangleR& rect) const noexcept {
    DrawTextureScaled(texture,rect, 0, false, 0, WHITE);
  }
  void draw_tooltip() const noexcept {
    auto mouse = MOUSE_POS;
    float startX, startY;
    float width = tooltip_x * UI_SCALE;
    float height = tooltip_y * UI_SCALE;
    if (mouse.x - width < 0) {
      startX = mouse.x + 10;
    } else {
      startX = mouse.x - width;
    }
    if (mouse.y - height < 0) {
      startY = mouse.y + 5;
    } else {
      startY = mouse.y - height;
    }

    DrawRectangleRounded({startX - 4, startY - 4, width + 8, height + 8}, 0.1,
                         ROUND_SEGMENTS, Colors::LightGrey);
    DrawRectangleRoundedLines({startX, startY, width, height}, 0.1, ROUND_SEGMENTS, 2,
                              rarity_to_color[rarity]);

    /* |-----------------------------------------------------|
     * |                     ITEM LEVEL                      |
     * |-----------------------------------------------------|
     */
    snprintf(text_buffer, 10, "ilvl:%d", level);
    DrawTextExR(MINECRAFT_ITALIC, text_buffer,
                {startX + 3 * UI_SCALE, startY + 2 * UI_SCALE}, 16 * UI_SCALE, 1,
                Colors::darkBackground);

    //quality
    snprintf(text_buffer, 10, "%d%%", quality);
    if (quality == 100) [[unlikely]] {
      DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                  {startX + width - 36 * UI_SCALE, startY + 2 * UI_SCALE}, 16 * UI_SCALE,
                  1, get_quality_color());
    } else {
      DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                  {startX + width - 29 * UI_SCALE, startY + 2 * UI_SCALE}, 16 * UI_SCALE,
                  1, get_quality_color());
    }

    //name
    DrawTextExR(MINECRAFT_BOLD, name.c_str(),
                {startX + 5 * UI_SCALE, startY + 40 * UI_SCALE}, 21 * UI_SCALE, 1,
                rarity_to_color[rarity]);

    //attributes
    float off_setX = 6 * UI_SCALE;
    float off_sety = 80 * UI_SCALE;
    float v_space = 15 * UI_SCALE;
    float h_space = 90 * UI_SCALE;
    float font_size = 16 * UI_SCALE;

    snprintf(text_buffer, 10, "INT: %d", (int)effects[INTELLIGENCE]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer, {startX + off_setX, startY + off_sety},
                font_size, 1, Colors::darkBackground);

    snprintf(text_buffer, 10, "WIS: %d", (int)effects[WISDOM]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX, startY + off_sety + v_space}, font_size, 1,
                Colors::darkBackground);
    snprintf(text_buffer, 10, "FOC: %d", (int)effects[FOCUS]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX, startY + off_sety + v_space * 2}, font_size, 1,
                Colors::darkBackground);

    snprintf(text_buffer, 10, "VIT: %d", (int)effects[VITALITY]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX + h_space, startY + off_sety}, font_size, 1,
                Colors::darkBackground);
    snprintf(text_buffer, 10, "AGI: %d", (int)effects[AGILITY]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX + h_space, startY + off_sety + v_space}, font_size, 1,
                Colors::darkBackground);
    snprintf(text_buffer, 10, "END: %d", (int)effects[ENDURANCE]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX + h_space, startY + off_sety + v_space * 2}, font_size,
                1, Colors::darkBackground);

    snprintf(text_buffer, 10, "STR: %d", (int)effects[STRENGTH]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX + h_space * 2, startY + +off_sety}, font_size, 1,
                Colors::darkBackground);
    snprintf(text_buffer, 10, "CHA: %d", (int)effects[CHARISMA]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX + h_space * 2, startY + off_sety + v_space}, font_size,
                1, Colors::darkBackground);
    snprintf(text_buffer, 10, "LUC: %d", (int)effects[LUCK]);
    DrawTextExR(MINECRAFT_REGULAR, text_buffer,
                {startX + off_setX + h_space * 2, startY + off_sety + v_space * 2},
                font_size, 1, Colors::darkBackground);

    //effects
    off_sety = 145 * UI_SCALE;
    DrawTextExR(MINECRAFT_BOLD, "Effects:", {startX + off_setX, startY + off_sety},
                font_size, 1, Colors::darkBackground);
    off_sety += 20;
    for (uint_fast32_t i = 9; i < WEAPON_DAMAGE; i++) {
      if (effects[i] != 0) {
        snprintf(text_buffer, 10, "%+0.2f", effects[i] * 100);
        std::string displayText = std::string(text_buffer) + " " + statToName[Stat(i)];
        DrawTextExR(MINECRAFT_REGULAR, displayText.c_str(),
                    {startX + off_setX, startY + off_sety}, font_size, 1,
                    Colors::darkBackground);
        off_sety += font_size;
      }
    }
    for (uint_fast32_t i = WEAPON_DAMAGE; i < STATS_ENDING; i++) {
      if (effects[i] != 0) {
        snprintf(text_buffer, 10, "%+0.2f", effects[i]);
        std::string displayText = std::string(text_buffer) + " " + statToName[Stat(i)];
        DrawTextExR(MINECRAFT_REGULAR, displayText.c_str(),
                    {startX + off_setX, startY + off_sety}, font_size, 1,
                    Colors::darkBackground);
        off_sety += font_size;
      }
    }

    //description
    off_sety = 240 * UI_SCALE;
    std::string wrappedText =  Util::WrapText(description,width-5,MINECRAFT_ITALIC,SCALE(15));
      DrawTextExR(MINECRAFT_ITALIC, wrappedText.c_str(), {startX + off_setX, startY + off_sety},
                SCALE(15), 0.5F, Colors::darkBackground);


    //durability
    snprintf(text_buffer, 10, "D:%d", durability);
    DrawTextExR(MINECRAFT_ITALIC, text_buffer,
                {startX + off_setX, startY + height - 15 * UI_SCALE}, 15 * UI_SCALE, 0.5F,
                Colors::darkBackground);

    //type
    DrawTextExR(MINECRAFT_ITALIC, type_to_string[type].c_str(),
                {startX + width / 2 - GetTextWidth(type_to_string[type].c_str()) / 2.0F,
                 startY + height - 15 * UI_SCALE},
                15 * UI_SCALE, 0.5F, Colors::darkBackground);

    //id
    snprintf(text_buffer, 10, "id:%d%d", id, type);
    DrawTextExR(MINECRAFT_ITALIC, text_buffer,
                {startX + width - 35 * UI_SCALE, startY + height - 15 * UI_SCALE},
                15 * UI_SCALE, 0.5F, Colors::darkBackground);
  }
  [[nodiscard]] inline Color get_quality_color() const noexcept {
    if (quality < 90) [[likely]] {
      return Colors::NormalGrey;
    } else if (quality == 100) {
      return Colors::highQuality;
    } else {
      return Colors::mediumQuality;
    }
  }

  inline static void scale(int quality, int level) {}
  [[nodiscard]] Item* scale_clone(int new_level, int new_quality) const noexcept {
    auto new_item = new Item(*this);
    return new_item;
  }
};
char Item::text_buffer[10];
#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
