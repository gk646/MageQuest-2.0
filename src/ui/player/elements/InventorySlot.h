#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
class InventorySlot;
inline static InventorySlot* DRAGGED_SLOT;
inline static InventorySlot* PLAYER_EQUIPPED;
inline static InventorySlot* PLAYER_BAG;
inline static int PLAYER_BAG_SIZE = 0;
struct InventorySlot {
  static constexpr int tool_tip_delay = 12;
  int tool_tip_counter = 0;
  int base_x = 0, base_y = 0;
  RectangleR hit_box = {0};
  Item* item = nullptr;
  ItemType item_type = ItemType::EMPTY;
  InventorySlot() = default;
  InventorySlot(int x, int y, ItemType item_type) noexcept
      : hit_box(x, y, 40, 40), base_x(x), base_y(y), item_type(item_type) {}
  void draw(float x, float y) noexcept {
    hit_box.x = (x + base_x) * UI_SCALE;
    hit_box.y = (y + base_y) * UI_SCALE;

    auto ptr = item;  //thread safety
    DrawRectangleRounded(hit_box, 0.4F, 40, Colors::mediumVeryLight);
    DrawRectangleRoundedLines(
        hit_box, 0.4F, 40, 2,
        ptr ? rarity_to_color[ptr->rarity] : Colors::darkBackground);

    if (ptr) {
      ptr->draw(hit_box);
      if (tool_tip_counter > tool_tip_delay) {
        TOOL_TIP_ITEM = ptr;
      }
    }
  }
  void DrawCharacterSlot(float x, float y) {
    draw(x, y);
    auto ptr = item;
    if (!ptr) return;
    sprintf(ptr->text_buffer, "%i", ptr->level);
    if (base_x < 175) {
      DrawRightAlignedText(
          MINECRAFT_BOLD, SCALE(14), ptr->text_buffer, hit_box.x - SCALE(4),
          hit_box.y + (hit_box.height - SCALE(14)) / 2, rarity_to_color[ptr->rarity]);
    } else {
      DrawTextExR(MINECRAFT_BOLD, ptr->text_buffer,
                  {hit_box.x + hit_box.width + SCALE(4),
                   hit_box.y + (hit_box.height - SCALE(14)) / 2},
                  SCALE(14), 0, rarity_to_color[ptr->rarity]);
    }
  }
  static void place_item_back() noexcept {
    if (DRAGGED_SLOT->item_type == ItemType::EMPTY) {
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    } else if (DRAGGED_SLOT->item_type == ItemType::BAG) {
      //TODO
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    } else {
      PLAYER_STATS.equip_item(DRAGGED_ITEM->effects);
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }
  }
  inline void draw_inventory_icons() const noexcept {
    switch (item_type) {
      case ItemType::HEAD:
        DrawTexturePro(textures::helm, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::CHEST:
        DrawTexturePro(textures::chest, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::PANTS:
        DrawTexturePro(textures::pants, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::BOOTS:
        DrawTexturePro(textures::boots, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::AMULET:
        DrawTexturePro(textures::amulet, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::RING:
        DrawTexturePro(textures::ring, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::RELIC:
        DrawTexturePro(textures::relic, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::ONE_HAND:
        DrawTexturePro(textures::weapon, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::TWO_HAND:
        DrawTexturePro(textures::weapon, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::OFF_HAND:
        DrawTexturePro(textures::offhand, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      case ItemType::BAG:
        DrawTexturePro(textures::bag, {0, 0, 40, 40}, hit_box, {0, 0}, 0, WHITE);
        break;
      default:
        break;
    }
  }
  void update_player_inv() {
    hit_box.height = 40 * UI_SCALE;
    hit_box.width = 40 * UI_SCALE;
    if (CheckCollisionPointRec(MOUSE_POS, hit_box)) {
      if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
          for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
            if (!PLAYER_BAG[i].item) {
              PLAYER_BAG[i].item = item;
              PLAYER_STATS.un_equip_item(item->effects);
              item = nullptr;
              break;
            }
          }
        } else {
          PLAYER_STATS.un_equip_item(item->effects);
          DRAGGED_ITEM = item;
          DRAGGED_SLOT = this;
          item = nullptr;
        }
      } else if (DRAGGED_ITEM && !item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                 item_type == DRAGGED_ITEM->type) {
        item = DRAGGED_ITEM;
        PLAYER_STATS.equip_item(item->effects);
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      } else if (DRAGGED_ITEM && item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                 DRAGGED_ITEM->type == item_type) {
        if (DRAGGED_SLOT->item_type == ItemType::EMPTY) {
          PLAYER_STATS.un_equip_item(item->effects);
        }
        PLAYER_STATS.equip_item(DRAGGED_ITEM->effects);
        DRAGGED_SLOT->item = item;
        item = DRAGGED_ITEM;
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      }
      tool_tip_counter++;
    } else {
      tool_tip_counter = 0;
    }
  }
  void update() noexcept {
    hit_box.height = 40 * UI_SCALE;
    hit_box.width = 40 * UI_SCALE;
    if (CheckCollisionPointRec(MOUSE_POS, hit_box)) {
      if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
          for (uint_fast32_t i = 0; i < 10; i++) {
            if (!PLAYER_EQUIPPED[i].item && PLAYER_EQUIPPED[i].item_type == item->type) {
              PLAYER_EQUIPPED[i].item = item;
              PLAYER_STATS.equip_item(item->effects);
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
                 (DRAGGED_SLOT->item_type == ItemType::EMPTY ||
                  DRAGGED_SLOT->item_type == item->type)) {
        if (DRAGGED_SLOT->item_type != ItemType::EMPTY) {
          PLAYER_STATS.equip_item(item->effects);
        }
        DRAGGED_SLOT->item = item;
        item = DRAGGED_ITEM;
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      }
      tool_tip_counter++;
    } else {
      tool_tip_counter = 0;
    }
  }
};

#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
