#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_



struct CharacterPanel : public Window {
  static constexpr int width = 400;
  static constexpr int left_x = 50;
  static constexpr int begin_y = 50;
  static constexpr int right_x = 300;
  static constexpr int vertical_gap = 65;
  std::array<InventorySlot, 10> equip_slots = {
      // left, from top to bottom
      InventorySlot(left_x, begin_y, ItemType::HEAD),
      InventorySlot(left_x, begin_y + vertical_gap, ItemType::CHEST),
      InventorySlot(left_x, begin_y + vertical_gap * 2, ItemType::PANTS),
      InventorySlot(left_x, begin_y + vertical_gap * 3, ItemType::BOOTS),

      //  right, from top to bottom
      InventorySlot(right_x, begin_y, ItemType::AMULET),
      InventorySlot(right_x, begin_y + vertical_gap, ItemType::RING),
      InventorySlot(right_x, begin_y + vertical_gap * 2, ItemType::RING),
      InventorySlot(right_x, begin_y + vertical_gap * 3, ItemType::RING),

      // bottom middle
      InventorySlot(width / 2 - SLOT_SIZE * 1.5F, begin_y + vertical_gap * 4,
                    ItemType::ONE_HAND),
      InventorySlot(width / 2 + SLOT_SIZE / 2, begin_y + vertical_gap * 4,
                    ItemType::OFF_HAND)};
  CharacterPanel() noexcept : Window(SCREEN_WIDTH*0.1F,SCREEN_HEIGHT*0.2F,width, 500, 25, "Character", KEY_C) {}
  void draw() {
    OPEN_CLOSE()
    DRAG_WINDOW()
    draw_window();
    for (auto& slot : equip_slots) {
      slot.draw(whole_window.x, whole_window.y);
      if (!slot.filled) {
        slot.draw_inventory_icons();
      }
    }
  }
  void update() noexcept {
    update_window();
    for (auto& slot : equip_slots) {
      slot.update_player_inv();
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERPANEL_H_
