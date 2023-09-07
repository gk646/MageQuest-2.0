#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
class InventorySlot;
inline InventorySlot* DRAGGED_SLOT;

struct InventorySlot {
  static constexpr int tool_tip_delay = 12;
  int tool_tip_counter = 0;
  int base_x, base_y = 0;
  Rectangle hit_box = {0};
  Item* item = nullptr;
  ItemType item_type = ItemType::EMPTY;
  InventorySlot(int x, int y, ItemType item_type) noexcept
      : hit_box(x, y, 40, 40), base_x(x), base_y(y), item_type(item_type) {}

  void draw(float x, float y) noexcept {
    hit_box.x = (x + base_x) * UI_SCALE;
    hit_box.y = (y + base_y) * UI_SCALE;
    DrawRectangleRounded(hit_box, 0.4F, 20, Colors::LightGrey);
    DrawRectangleRoundedLines(hit_box, 0.4F, 20, 2, Colors::darkBackground);
    if (item) {
      item->draw(hit_box);
      if (tool_tip_counter > tool_tip_delay) {
        item->draw_tooltip(hit_box);
      }
    }
  }
  static void place_item_back() noexcept{
    if(DRAGGED_SLOT->item_type == ItemType::EMPTY ){
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }else if(DRAGGED_SLOT->item_type == ItemType::BAG){
      //TODO
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }else{
      PLAYER_STATS.equip_item(DRAGGED_ITEM->effects);
      DRAGGED_SLOT->item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }
  }
  inline void draw_inventory_icons() const noexcept {
    switch (item_type) {
      case ItemType::HEAD:
        DrawTexturePro(textures::helm, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::CHEST:
        DrawTexturePro(textures::chest, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::PANTS:
        DrawTexturePro(textures::pants, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::BOOTS:
        DrawTexturePro(textures::boots, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::AMULET:
        DrawTexturePro(textures::amulet, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::RING:
        DrawTexturePro(textures::ring, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::RELIC:
        DrawTexturePro(textures::relic, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::ONE_HAND:
        DrawTexturePro(
            textures::weapon, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
            WHITE);  // Assuming one-hand weapons use the same texture
        break;
      case ItemType::TWO_HAND:
        DrawTexturePro(
            textures::weapon, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
            WHITE);  // Assuming two-hand weapons use the same texture
        break;
      case ItemType::OFF_HAND:
        DrawTexturePro(textures::offhand, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      case ItemType::BAG:
        DrawTexturePro(textures::bag, {0, 0, 40, 40}, hit_box, {0, 0}, 0,
                       WHITE);
        break;
      default:
        break;
    }
  }
  void update_player_inv() {
    hit_box.height = 40 * UI_SCALE;
    hit_box.width = 40 * UI_SCALE;
    if (CheckCollisionPointRec(GetMousePosition(), hit_box)) {
      if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        PLAYER_STATS.un_equip_item(item->effects);
        DRAGGED_ITEM = item;
        DRAGGED_SLOT = this;
        item = nullptr;
      } else if (DRAGGED_ITEM && !item &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT) && item_type == DRAGGED_ITEM->type) {
        item = DRAGGED_ITEM;
        PLAYER_STATS.equip_item(item->effects);
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      } else if (DRAGGED_ITEM && item &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT) && DRAGGED_SLOT->item_type == item->type) {
        DRAGGED_SLOT->item = item;
        item = DRAGGED_ITEM;
        PLAYER_STATS.equip_item(DRAGGED_ITEM->effects);
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
    if (CheckCollisionPointRec(GetMousePosition(), hit_box)) {
      if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DRAGGED_ITEM = item;
        DRAGGED_SLOT = this;
        item = nullptr;
      } else if (DRAGGED_ITEM && !item &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        item = DRAGGED_ITEM;
        DRAGGED_SLOT = nullptr;
        DRAGGED_ITEM = nullptr;
      } else if (DRAGGED_ITEM && item &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT) && DRAGGED_SLOT->item_type == item->type) {
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
