#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ITEMSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ITEMSLOT_H_

//A wrapper for an item
struct ItemSlot {
  RectangleR hitBox = {0};
  Item* item = nullptr;
  uint16_t baseX = 0, baseY = 0;
  ItemType slotType = ItemType::EMPTY;
  int8_t toolTipHoverTicks = 0;
  uint8_t baseWidth = 40, baseHeight = 40;
  ItemSlot() = default;
  ItemSlot(int x, int y, ItemType item_type, float width = 40, float height = 40) noexcept
      : hitBox((uint16_t)x, (uint16_t)y, width, height),
        baseX(x),
        baseY(y),
        slotType(item_type),
        baseWidth(width),
        baseHeight(height) {}
  inline bool operator<(const ItemSlot& other) const {
    if (!item) return other.item != nullptr;
    if (!other.item) return false;
    return *item < *other.item;
  }

  inline bool operator>(const ItemSlot& other) const {
    if (!item) return false;
    if (!other.item) return item != nullptr;
    return *item > *other.item;
  }
  //General update method for all bag slots
  void Draw(float x, float y) noexcept {
    hitBox.x = (x + (float)baseX) * UI_SCALE;
    hitBox.y = (y + (float)baseY) * UI_SCALE;

    auto ptr = item;  //thread safety
    DrawRectangleRounded(hitBox, 0.4F, 40, Colors::mediumVeryLight);
    DrawRectangleRoundedLines(
        hitBox, 0.4F, 40, 2, ptr ? rarity_to_color[ptr->rarity] : Colors::darkBackground);

    if (ptr) {
      ptr->Draw(hitBox);
      if (toolTipHoverTicks < 0) {
        TOOL_TIP_ITEM = ptr;
      }
    }
  }
  //Method for the character slots
  void DrawCharacterSlot(float x, float y) {
    Draw(x, y);
    auto ptr = item;
    if (!ptr) return;
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%i", ptr->level);
    if (baseX < 175) {
      Util::DrawRightAlignedText(
          MINECRAFT_BOLD, SCALE(14), TEXT_BUFFER, hitBox.x - SCALE(4),
          hitBox.y + (hitBox.height - SCALE(14)) / 2, rarity_to_color[ptr->rarity]);
    } else {
      DrawTextExR(MINECRAFT_BOLD, TEXT_BUFFER,
                  {hitBox.x + hitBox.width + SCALE(4),
                   hitBox.y + (hitBox.height - SCALE(14)) / 2},
                  SCALE(14), 0, rarity_to_color[ptr->rarity]);
    }
  }

  //Logic for exchanging items
 public:
  void UpdateCharacterSlots() noexcept;
  void Update() noexcept;

 public:
  inline static void RecoverDraggedItem() noexcept {
    if (DRAGGED_SLOT->slotType == ItemType::EMPTY) {
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    } else if (DRAGGED_SLOT->slotType == ItemType::BAG) {
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    } else {
      PLAYER_STATS.EquipItem(DRAGGED_ITEM->effects);
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }
  }
  //Draws the item type indicators for the empty inventory slots
  inline void DrawBackGroundIcons() const noexcept {
    if (item) return;
    switch (slotType) {
      case ItemType::HEAD:
        DrawTexturePro(textures::helm, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::CHEST:
        DrawTexturePro(textures::chest, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::PANTS:
        DrawTexturePro(textures::pants, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::BOOTS:
        DrawTexturePro(textures::boots, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::AMULET:
        DrawTexturePro(textures::amulet, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::RING:
        DrawTexturePro(textures::ring, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::RELIC:
        DrawTexturePro(textures::relic, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::ONE_HAND:
      case ItemType::TWO_HAND:
        DrawTexturePro(textures::weapon, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::OFF_HAND:
        DrawTexturePro(textures::offhand, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      case ItemType::BAG:
        DrawTexturePro(textures::bag, {0, 0, 40, 40}, hitBox, {0, 0}, 0, WHITE);
        break;
      default:
        break;
    }
  }

 private:
  [[nodiscard]] inline bool NoSwitchDragRules(const ItemSlot& ptr) const noexcept {
    if (ptr.item) return false;
    if (item->type == ItemType::OFF_HAND) {
      return ptr.slotType == ItemType::OFF_HAND &&
             (PLAYER_EQUIPPED[8].item == nullptr ||
              PLAYER_EQUIPPED[8].item->type == ItemType::ONE_HAND);

    } else if (item->type == ItemType::TWO_HAND) {
      return ptr.slotType == ItemType::ONE_HAND && PLAYER_EQUIPPED[9].item == nullptr;
    } else {
      return ptr.slotType == item->type;
    }
  }
  [[nodiscard]] inline bool CharacterSlotDropRules() const noexcept {
    if (DRAGGED_ITEM->type == ItemType::OFF_HAND) {
      return slotType == ItemType::OFF_HAND &&
             (PLAYER_EQUIPPED[8].item == nullptr ||
              PLAYER_EQUIPPED[8].item->type == ItemType::ONE_HAND);
    } else if (DRAGGED_ITEM->type == ItemType::TWO_HAND) {
      return slotType == ItemType::ONE_HAND && PLAYER_EQUIPPED[9].item == nullptr;
    } else {
      return slotType == DRAGGED_ITEM->type;
    }
  }
  [[nodiscard]] inline bool BagSlotDropRuleSwitch() const noexcept {
    if (DRAGGED_SLOT->slotType == ItemType::EMPTY) return true;
    if (item->type == ItemType::TWO_HAND) {
      return DRAGGED_ITEM->type == ItemType::ONE_HAND &&
             PLAYER_EQUIPPED[9].item == nullptr;
    } else {
      return item->type == DRAGGED_SLOT->slotType;
    }
  }
};

void Util::SelectionSortInventorySlot(ItemSlot* arr, uint_32_cx len, bool ascending) {
  uint_32_cx index;
  if (ascending) {
    for (uint_32_cx i = 0; i < len; i++) {
      ItemSlot low = arr[i];
      index = i;
      for (uint_32_cx j = i + 1; j < len; j++) {
        if (arr[j] < low) {
          low = arr[j];
          index = j;
        }
      }
      std::swap(arr[i].item, arr[index].item);
    }
  } else {
    for (uint_32_cx i = 0; i < len; i++) {
      ItemSlot high = arr[i];
      index = i;
      for (uint_32_cx j = i + 1; j < len; j++) {
        if (arr[j] > high) {
          high = arr[j];
          index = j;
        }
      }
      std::swap(arr[i].item, arr[index].item);
    }
  }
}
int ItemSet::CountEquippedSetItemsOfSet(const ItemSet& itemSet) noexcept {
  int count = 0;
  bool equippedAny = false;
  for (int i = 0; i < 10; i++) {
    if (IsPartOfItemSet(PLAYER_EQUIPPED[i].item, itemSet)) {
      equippedAny = true;
      for (int j = 0; j < 10; j++) {
        if (j != i && PLAYER_EQUIPPED[j].item &&
            *PLAYER_EQUIPPED[j].item == *PLAYER_EQUIPPED[i].item) {
          goto continueOuterLoop;
        }
      }
      count++;
    }
  continueOuterLoop:;
  }
  if (equippedAny) count = std::max(1, count);
  return count;
}
void Item::DrawSetItemToolTip(float x, float y, float& offSet) const noexcept {
  if (itemSet == ItemSetNum::NO_SET) return;
  const auto& set = ITEM_SETS[(int)itemSet];
  int equippedCount = ItemSet::CountEquippedSetItemsOfSet(set);
  snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Set: %s %d/%d", set.title.c_str(),
           equippedCount, set.thresholds[set.bonusCount - 1]);
  DrawTextExR(MINECRAFT_BOLD, TEXT_BUFFER, {x + 5, y + offSet}, 15, 0.5F,
              Colors::darkBackground);

  offSet += 17;
  for (int i = 0; i < set.bonusCount; i++) {
    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "(%d)", set.thresholds[i]);
    DrawTextExR(MINECRAFT_ITALIC, TEXT_BUFFER, {x + 2, y + offSet}, 14, 0.5F,
                Colors::darkBackground);
    int lineBreaks = 0;
    auto wrappedText = Util::WrapText(set.descriptions[i], Item::TOOL_TIP_WIDTH - 25,
                                      MINECRAFT_ITALIC, 14, &lineBreaks);
    DrawTextExR(MINECRAFT_ITALIC, wrappedText.c_str(), {x + 25, y + offSet}, 14, 0.5F,
                equippedCount >= set.thresholds[i] ? Colors::descriptionOrange
                                                   : Color{90, 105, 136, 120});
    offSet += 14 * lineBreaks;
  }
  offSet += 14;
}
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ITEMSLOT_H_
