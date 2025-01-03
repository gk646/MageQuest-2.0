#ifndef MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
#define MAGEQUEST_SRC_GAMEPLAY_ITEM_H_


struct Item {
  static constexpr int TOOL_TIP_WIDTH = 260;
  static constexpr int TOOL_TIP_HEIGHT = 276;
  inline static char textBuffer[10];
  float effects[STATS_ENDING] = {0};
  std::string name;
  std::string description;
  Texture texture;
  uint8_t quality = 70;
  uint8_t durability = 100;
  uint8_t level = 1;
  uint8_t id = 0;
  ItemRarity rarity = ItemRarity::NORMAL;
  ItemType type = ItemType::RING;
  ItemSetNum itemSet = ItemSetNum::NO_SET;
  //Creating the item
  Item(int id, std::string name, ItemRarity rarity, ItemType type,
       std::string description, const Texture& texture)
      : id(id),
        name(std::move(name)),
        rarity(rarity),
        type(type),
        description(std::move(description)),
        texture(texture) {}
  Item(const Item& other)
      : id(other.id),
        quality(other.quality),
        durability(other.durability),
        level(other.level),
        name(other.name),
        description(other.description),
        texture(other.texture),
        rarity(other.rarity),
        type(other.type),
        itemSet(other.itemSet) {
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
    itemSet = other.itemSet;
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
        type(other.type),
        itemSet(other.itemSet) {
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
    itemSet = other.itemSet;
    std::move(std::begin(other.effects), std::end(other.effects), std::begin(effects));
    return *this;
  }
  inline bool operator==(const Item& other) const {
    return id == other.id && type == other.type;
  }
  inline bool operator!=(const Item& other) const {
    return id != other.id || type != other.type;
  }
  inline bool operator<(const Item& other) const { return rarity < other.rarity; }
  inline bool operator>(const Item& other) const { return rarity > other.rarity; }
  void Draw(const RectangleR& rect) const noexcept {
    DrawTextureScaled(texture, rect, 0, false, 0, WHITE);
  }
  //Draws the item tooltip correctly aligned, so It's not outside screen bounds
  void DrawToolTip() const noexcept {
    auto mouse = GetMousePosition();
    float startX, startY;
    float width = TOOL_TIP_WIDTH * GAME_SETTINGS.uiScale;

    int lineBreaks = 0;
    std::string wrappedText =
        Util::WrapText(description, width - 5, MINECRAFT_ITALIC, 15, &lineBreaks);

    float setHeight = 0;
    if (itemSet != ItemSetNum::NO_SET) {
      setHeight = ITEM_SETS[(int)itemSet].toolTipHeight;
    }
    float height = TOOL_TIP_HEIGHT + 15.0F * (float)lineBreaks + setHeight;

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
                         GAME_SETTINGS.roundSegments, Colors::LightGrey);
    DrawRectangleRoundedLines({startX, startY, width, height}, 0.1,
                              GAME_SETTINGS.roundSegments, 2, rarity_to_color[rarity]);

    /* |-----------------------------------------------------|
     * |                     ITEM LEVEL                      |
     * |-----------------------------------------------------|
     */
    snprintf(textBuffer, 10, "ilvl:%d", level);
    DrawTextExR(MINECRAFT_ITALIC, textBuffer, {startX + 3, startY + 2}, 16, 1,
                Colors::darkBackground);

    //quality
    snprintf(textBuffer, 10, "%d%%", quality);
    if (quality == 100) [[unlikely]] {
      DrawTextExR(MINECRAFT_REGULAR, textBuffer, {startX + width - 36, startY + 2}, 16, 1,
                  GetQualityColor());
    } else {
      DrawTextExR(MINECRAFT_REGULAR, textBuffer, {startX + width - 29, startY + 2}, 16, 1,
                  GetQualityColor());
    }

    //name
    DrawTextExR(MINECRAFT_BOLD, name.c_str(), {startX + 5, startY + 40}, 21, 1,
                rarity_to_color[rarity]);

    //attributes
    float off_setX = 6;
    float off_sety = 80;
    float v_space = 15;
    float h_space = 90;
    float font_size = 16;

    snprintf(textBuffer, 10, "INT: %d", (int)effects[INTELLIGENCE]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer, {startX + off_setX, startY + off_sety},
                font_size, 1, Colors::darkBackground);

    snprintf(textBuffer, 10, "WIS: %d", (int)effects[WISDOM]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX, startY + off_sety + v_space}, font_size, 1,
                Colors::darkBackground);
    snprintf(textBuffer, 10, "FOC: %d", (int)effects[FOCUS]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX, startY + off_sety + v_space * 2}, font_size, 1,
                Colors::darkBackground);

    snprintf(textBuffer, 10, "VIT: %d", (int)effects[VITALITY]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX + h_space, startY + off_sety}, font_size, 1,
                Colors::darkBackground);
    snprintf(textBuffer, 10, "AGI: %d", (int)effects[AGILITY]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX + h_space, startY + off_sety + v_space}, font_size, 1,
                Colors::darkBackground);
    snprintf(textBuffer, 10, "END: %d", (int)effects[ENDURANCE]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX + h_space, startY + off_sety + v_space * 2}, font_size,
                1, Colors::darkBackground);

    snprintf(textBuffer, 10, "STR: %d", (int)effects[STRENGTH]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX + h_space * 2, startY + +off_sety}, font_size, 1,
                Colors::darkBackground);
    snprintf(textBuffer, 10, "CHA: %d", (int)effects[CHARISMA]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX + h_space * 2, startY + off_sety + v_space}, font_size,
                1, Colors::darkBackground);
    snprintf(textBuffer, 10, "LUC: %d", (int)effects[LUCK]);
    DrawTextExR(MINECRAFT_REGULAR, textBuffer,
                {startX + off_setX + h_space * 2, startY + off_sety + v_space * 2},
                font_size, 1, Colors::darkBackground);

    //effects
    off_sety = 145;
    DrawTextExR(MINECRAFT_BOLD, "Effects:", {startX + off_setX, startY + off_sety},
                font_size, 1, Colors::darkBackground);
    off_sety += 20;
    for (int i = 9; i < WEAPON_DAMAGE; i++) {
      if (effects[i] != 0) {
        snprintf(textBuffer, 10, "%+0.2f", effects[i] * 100);
        std::string displayText = std::string(textBuffer) + " " + statToName[Stat(i)];
        DrawTextExR(MINECRAFT_REGULAR, displayText.c_str(),
                    {startX + off_setX, startY + off_sety}, font_size, 1,
                    Colors::darkBackground);
        off_sety += font_size;
      }
    }

    for (int i = WEAPON_DAMAGE + 1; i < STATS_ENDING; i++) {
      if (effects[i] != 0) {
        if (std::round(effects[i]) == effects[i]) {
          snprintf(textBuffer, 10, "+%d", (int)effects[i]);
        } else {
          snprintf(textBuffer, 10, "%+0.2f", effects[i]);
        }
        std::string displayText = std::string(textBuffer) + " " + statToName[Stat(i)];
        DrawTextExR(MINECRAFT_REGULAR, displayText.c_str(),
                    {startX + off_setX, startY + off_sety}, font_size, 1,
                    Colors::darkBackground);
        off_sety += font_size;
      }
    }

    off_sety = 240;
    //Set item
    DrawSetItemToolTip(startX, startY, off_sety);

    //description
    DrawTextExR(MINECRAFT_ITALIC, wrappedText.c_str(),
                {startX + off_setX, startY + off_sety}, 15, 0.5F, Colors::darkBackground);

    //durability
    snprintf(textBuffer, 10, "D:%d", durability);
    DrawTextExR(MINECRAFT_ITALIC, textBuffer, {startX + off_setX, startY + height - 15},
                15, 0.5F, Colors::darkBackground);

    //type
    DrawTextExR(
        MINECRAFT_ITALIC, type_to_string[type].c_str(),
        {startX + width / 2 - (float)GetTextWidth(type_to_string[type].c_str()) / 2.0F,
         startY + height - 15},
        15, 0.5F, Colors::darkBackground);

    //id
    snprintf(textBuffer, 10, "id:%d%d", id, (int)type);
    DrawTextExR(MINECRAFT_ITALIC, textBuffer, {startX + width - 35, startY + height - 15},
                15, 0.5F, Colors::darkBackground);
  }
  [[nodiscard]] inline Color GetQualityColor() const noexcept {
    if (quality < 90) [[likely]] {
      return Colors::NormalGrey;
    } else if (quality == 100) {
      return Colors::highQuality;
    } else {
      return Colors::mediumQuality;
    }
  }
  //Returns a Clone of the base item that matches the given values
  inline static Item* FindBaseItemClone(int id, ItemType type, int quality, int level);
  //Parses the effects column of an entry in the form:"35:12.000000;" and adds it on top of existing values
  inline static void ParseEffectText(float* arr, const unsigned char* ptr) {
    if (!ptr) return;
    std::stringstream ss(reinterpret_cast<const char*>(ptr));
    std::string pair;
    while (std::getline(ss, pair, ';')) {
      size_t sep = pair.find(':');
      if (sep == std::string::npos) continue;
      arr[std::stoi(pair.substr(0, sep))] += std::stof(pair.substr(sep + 1));
    }
  }
  //Parses and adds the attribute values (STR5) from the given string to the float array
  inline static void ParseAttributeStats(float* arr, const std::string& input) {
    std::regex pattern(R"(([a-zA-Z]+)([-+]?\d+))");
    for (std::sregex_iterator it(input.begin(), input.end(), pattern), end; it != end;
         ++it) {
      const std::string& attribute = (*it)[1].str();
      if (attrToStat.count(attribute)) {
        arr[attrToStat[attribute]] += (float)std::stoi((*it)[2].str());
      }
    }
  }
  //Sets the correct weapon damage
  inline void SetWeaponDamage() noexcept {
    if (type == ItemType::TWO_HAND) {
      effects[WEAPON_DAMAGE] =
          (float)level / 100.0F + std::min((float)rarity, 4.0F) / 100.0F;
    } else if (type == ItemType::OFF_HAND || type == ItemType::ONE_HAND) {
      effects[WEAPON_DAMAGE] =
          ((float)level / 100.0F + std::min((float)rarity, 4.0F) / 100.0F) / 2.0F;
    }
  }

 private:
  void DrawSetItemToolTip(float x, float y, float& offset) const noexcept;
};

bool ItemSet::IsPartOfItemSet(Item* item, const ItemSet& itemSet) noexcept {
  if (!item) return false;
  for (const auto itemId : itemSet.items) {
    if (item->id == itemId.id && item->type == itemId.type) {
      return true;
    }
  }
  return false;
}

#endif  //MAGEQUEST_SRC_GAMEPLAY_ITEM_H_
