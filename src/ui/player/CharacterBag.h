#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_

struct CharacterBag final : public Window {
  static constexpr int width = 420;
  static constexpr int spacing_x = SLOT_SIZE + 5;
  static constexpr int spacing_y = SLOT_SIZE + 5;
  static constexpr int per_row = width / spacing_x;
  static constexpr int offset_x = 15;
  static constexpr int offset_y = 50;

  explicit CharacterBag(int slots) noexcept
      : Window(SCREEN_WIDTH * 0.6F, SCREEN_HEIGHT * 0.6F, width, 300, 20,
               "Bags", KEY_B) {
    add_slots(slots);
    CHAR_BAG[0].item = new Item("hello");
    CHAR_BAG[1].item = new Item("byebye");
  }

  void draw() noexcept {
    OPEN_CLOSE()
    DRAG_WINDOW()
    draw_window();
    for (auto& slot : CHAR_BAG) {
      slot.draw(whole_window.x, whole_window.y);
    }
  }
  void update() noexcept {
    update_window();
    for (auto& slot : CHAR_BAG) {
      slot.update();
    }
  }
  inline void add_slots(int n) noexcept {
    int exist_x = CHAR_BAG.size() % per_row;
    int exist_y = CHAR_BAG.size() / per_row;
    for (uint_fast32_t i = 0; i < n; i++) {
      CHAR_BAG.emplace_back(offset_x + spacing_x * exist_x,
                            offset_y + spacing_y * exist_y, ItemType::EMPTY);
      exist_x = CHAR_BAG.size() % per_row;
      exist_y = CHAR_BAG.size() / per_row;
    }
  }

  static void add_item(Item* new_item) noexcept {
    for (auto& slot : CHAR_BAG) {
      if (!slot.item) {
        slot.item = new_item;
        break;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
