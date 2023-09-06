#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_

struct CharacterBag final : public Window {
  static constexpr int width = 420;
  static constexpr int spacing_x = SLOT_SIZE + 5;
  static constexpr int spacing_y = SLOT_SIZE + 5;
  static constexpr int per_row = width / spacing_x ;
  static constexpr int offset_x = 15;
  static constexpr int offset_y = 50;
  std::vector<InventorySlot> bag;
  explicit CharacterBag(int slots) noexcept
      : Window(SCREEN_WIDTH*0.6F,SCREEN_HEIGHT*0.6F,width, 300, 20, "Bags", KEY_B) {
    add_slots(slots);
    bag[1].add_item(Item());
  }

   void draw() noexcept {
    OPEN_CLOSE()
    DRAG_WINDOW()
    draw_window();
    for (auto& slot : bag) {
     slot.draw(whole_window.x, whole_window.y);
    }
  }

  void update() noexcept {
    update_window();
    for (auto& slot : bag) {
      slot.update();
    }
  }

  inline void add_slots(int n) noexcept {
    int exist_x = bag.size() % per_row;
    int exist_y = bag.size() / per_row;
    for (uint_fast32_t i = 0; i < n; i++) {
      bag.emplace_back(offset_x + spacing_x * exist_x, offset_y + spacing_y * exist_y,
                       ItemType::EMPTY);
       exist_x = bag.size() % per_row;
       exist_y = bag.size() / per_row;
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
