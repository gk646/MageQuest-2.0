#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_

struct CharacterBag final : public Window {
  static constexpr int width = 420;
  static constexpr int spacing_x = SLOT_SIZE + 5;
  static constexpr int spacing_y = SLOT_SIZE + 5;
  static constexpr int per_row = width / spacing_x;
  static constexpr int offset_x = 15;
  static constexpr int offset_y = 50;
  static constexpr int max_slots = 40;
  explicit CharacterBag() noexcept
      : Window(SCREEN_WIDTH * 0.6F, SCREEN_HEIGHT * 0.6F, width, 300, 20, "Bags", KEY_B) {
    PLAYER_BAG = new InventorySlot[max_slots];
    cxstructs::now(2);
  }

  void draw() noexcept {
    WINDOW_LOGIC()
    draw_window();
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      PLAYER_BAG[i].draw(whole_window.x, whole_window.y);
    }
  }
  void update() noexcept {
   WINDOW_UPDATE();
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      PLAYER_BAG[i].update();
    }
  }
  inline static void add_slots(int n) noexcept {
    int exist_x = PLAYER_BAG_SIZE % per_row;
    int exist_y = PLAYER_BAG_SIZE / per_row;
    for (uint_fast32_t i = 0; i < n; i++) {
      RectangleR rect = {(float)offset_x + spacing_x * exist_x,
                         (float)offset_y + spacing_y * exist_y, 45, 45};
      PLAYER_BAG[i].hit_box = rect;
      PLAYER_BAG[i].base_x = rect.x;
      PLAYER_BAG[i].base_y = rect.y;
      PLAYER_BAG_SIZE++;
      exist_x = PLAYER_BAG_SIZE % per_row;
      exist_y = PLAYER_BAG_SIZE / per_row;
    }
  }
  static void add_item(Item* new_item) noexcept {
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      if (!PLAYER_BAG[i].item) {
        PLAYER_BAG[i].item = new_item;
        break;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
