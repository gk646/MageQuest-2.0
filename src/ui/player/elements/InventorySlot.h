#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
class InventrorySlot;
inline InventrorySlot* DRAGGED_SLOT;

struct InventorySlot {
  static constexpr int tool_tip_delay = 12;
  bool filled = false;
  int tool_tip_counter = 0;
  int base_x, base_y = 0;
  Rectangle hit_box = {0};
  Item item;
  ItemType item_type;
  InventorySlot(int x, int y, ItemType item_type) noexcept
      : hit_box(x, y, 40, 40), base_x(x), base_y(y), item_type(item_type) {}

  void draw(float x, float y) noexcept {
    hit_box.x = (x + base_x) * UI_SCALE;
    hit_box.y = (y + base_y) * UI_SCALE;
    DrawRectangleRounded(hit_box, 0.4F, 20, Colors::LightGrey);
    DrawRectangleRoundedLines(hit_box, 0.4F, 20, 2, Colors::darkBackground);
    if (filled) {
      item.draw(hit_box);
      if (tool_tip_counter > tool_tip_delay) {
        item.draw_tooltip(hit_box);
      }
    }
  }
  inline void add_item(const Item& new_item) noexcept {
    item = new_item;
    filled = true;
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
  void update_player_inv(){
    hit_box.height = 40 * UI_SCALE;
    hit_box.width = 40 * UI_SCALE;
    if (CheckCollisionPointRec(GetMousePosition(), hit_box)) {
      if (filled && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DRAGGED_ITEM = &item;
        filled = false;
        WINDOW_FOCUSED = true;
        PLAYER_STATS.un_equip_item(item.effects);
      } else if (DRAGGED_ITEM && !filled &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT) && DRAGGED_ITEM->type == item_type) {
        item = std::move(*DRAGGED_ITEM);
        filled = true;
        DRAGGED_ITEM = nullptr;
        PLAYER_STATS.equip_item(item.effects);
      } else if (DRAGGED_ITEM && filled &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
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
      if (filled && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DRAGGED_ITEM = &item;
        filled = false;
        WINDOW_FOCUSED = true;
      } else if (DRAGGED_ITEM && !filled &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        item = std::move(*DRAGGED_ITEM);
        filled = true;
        DRAGGED_ITEM = nullptr;
      } else if (DRAGGED_ITEM && filled &&
                 !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      }
      tool_tip_counter++;
    } else {
      tool_tip_counter = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_INVENTORYSLOT_H_
