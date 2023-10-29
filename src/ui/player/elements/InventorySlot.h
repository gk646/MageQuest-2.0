#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
class InventorySlot;
inline static InventorySlot* DRAGGED_SLOT;
inline static InventorySlot* PLAYER_EQUIPPED;
inline static InventorySlot* PLAYER_BAG;
//A wrapper
struct InventorySlot {
  RectangleR hitBox = {0};
  Item* item = nullptr;
  uint16_t baseX = 0, baseY = 0;
  ItemType slotType = ItemType::EMPTY;
  int8_t toolTipHoverTicks = 0;
  uint8_t baseWidth = 40, baseHeight = 40;
  InventorySlot() = default;
  InventorySlot(int x, int y, ItemType item_type, float width = 40,
                float height = 40) noexcept
      : hitBox((uint16_t)x, (uint16_t)y, width, height),
        baseX(x),
        baseY(y),
        slotType(item_type),
        baseWidth(width),
        baseHeight(height) {}
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

  static void RecoverDraggedItem() noexcept {
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
  void UpdateCharacterSlots() {
    hitBox.height = baseWidth * UI_SCALE;
    hitBox.width = baseHeight * UI_SCALE;
    if (CheckCollisionPointRec(MOUSE_POS, hitBox)) {
      if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
          for (uint_fast32_t i = 0; i < PLAYER_STATS.effects[BAG_SLOTS]; i++) {
            if (!PLAYER_BAG[i].item) {
              PLAYER_BAG[i].item = item;
              PLAYER_STATS.UnEquipItem(item->effects);
              item = nullptr;
              break;
            }
          }
        } else {
          PLAYER_STATS.UnEquipItem(item->effects);
          DRAGGED_ITEM = item;
          DRAGGED_SLOT = this;
          item = nullptr;
        }
      } else if (DRAGGED_ITEM && !item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                 slotType == DRAGGED_ITEM->type) {
        item = DRAGGED_ITEM;
        PlaySoundR(sound::equip);
        PLAYER_STATS.EquipItem(item->effects);
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      } else if (DRAGGED_ITEM && item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                 DRAGGED_ITEM->type == slotType) {
        if (DRAGGED_SLOT->slotType == ItemType::EMPTY) {
          PLAYER_STATS.UnEquipItem(item->effects);
        }
        PlaySoundR(sound::equip);
        PLAYER_STATS.EquipItem(DRAGGED_ITEM->effects);
        DRAGGED_SLOT->item = item;
        item = DRAGGED_ITEM;
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      }
      toolTipHoverTicks = std::max(toolTipHoverTicks - 1, -1);
    } else {
      toolTipHoverTicks = 12;
    }
  }
  void Update() noexcept {
    hitBox.height = baseWidth * UI_SCALE;
    hitBox.width = baseHeight * UI_SCALE;
    if (CheckCollisionPointRec(MOUSE_POS, hitBox)) {
      if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
          for (uint_fast32_t i = 0; i < 10; i++) {
            if (!PLAYER_EQUIPPED[i].item && PLAYER_EQUIPPED[i].slotType == item->type) {
              PLAYER_EQUIPPED[i].item = item;
              PLAYER_STATS.EquipItem(item->effects);
              item = nullptr;
              break;
            }
          }
        } else {
          DRAGGED_ITEM = item;
          DRAGGED_SLOT = this;
          item = nullptr;
        }
      } else if (DRAGGED_ITEM && !item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        item = DRAGGED_ITEM;
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      } else if (DRAGGED_ITEM && item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                 (DRAGGED_SLOT->slotType == ItemType::EMPTY ||
                  DRAGGED_SLOT->slotType == item->type)) {
        if (DRAGGED_SLOT->slotType != ItemType::EMPTY) {
          PLAYER_STATS.EquipItem(item->effects);
        }
        DRAGGED_SLOT->item = item;
        item = DRAGGED_ITEM;
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      }
      toolTipHoverTicks = std::max(toolTipHoverTicks - 1, -1);
    } else {
      toolTipHoverTicks = 12;
    }
  }
};

#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
