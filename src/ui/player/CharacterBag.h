
#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_

struct CharacterBag final : public Window {
  static constexpr int width = 420;
  static constexpr int spacing_x = SLOT_SIZE + 1;
  static constexpr int spacing_y = SLOT_SIZE + 2;
  static constexpr int per_row = width / spacing_x;
  static constexpr int offset_x = 15;
  static constexpr int offset_y = 50;
  static constexpr int max_slots = 40;
  static inline char HEADER[] = "Bags";
  explicit CharacterBag() noexcept
      : Window(SCREEN_WIDTH*0.80 - width, SCREEN_HEIGHT * 0.6F, width, 300, 20, HEADER, KEY_B) {
    PLAYER_BAG = new InventorySlot[max_slots];
    cxstructs::now(2);
  }

  void Draw() noexcept {
    if (IsKeyPressed(windowOpenKey)) {
      if (isWindowOpen) {
        PlaySoundR(sound::close_inventory);
        isWindowOpen = false;
      } else {
        PlaySoundR(sound::open_inventory);
        isWindowOpen = true;
      }
      isDragged = false;
    }
    if (!isWindowOpen) {
      return;
    }
    DRAG_WINDOW()
    DrawWindow();
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      PLAYER_BAG[i].draw(wholeWindow.x, wholeWindow.y);
    }
  }
  void Update() noexcept {
    WINDOW_UPDATE();
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      PLAYER_BAG[i].update();
    }
  }
  inline static void add_slots(int n) noexcept {
    //todo add dynamic inventory size based on slots
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
  static bool AddItem(Item* new_item) noexcept {
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      if (!PLAYER_BAG[i].item && &PLAYER_BAG[i] != DRAGGED_SLOT) {
        PLAYER_BAG[i].item = new_item;
        GAME_STATISTICS.PickedUpItem(new_item->rarity);
        return true;
      }
    }
    return false;
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
