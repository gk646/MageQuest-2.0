
#ifndef MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
#define MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
#include "../elements/SlideComponent.h"
#include "BagPanel.h"

struct CharacterBag final : public Window {
  static constexpr int width = 425;
  static constexpr int spacing_x = SLOT_SIZE + 1;
  static constexpr int spacing_y = SLOT_SIZE + 1;
  static constexpr int per_row = width / spacing_x;
  static constexpr int offset_x = 10;
  static constexpr int offset_y = 60;
  static constexpr int max_slots = 40;
  static inline char HEADER[] = "Bags";
  BagPanel bagPanel;
  explicit CharacterBag() noexcept
      : Window(SCREEN_WIDTH * 0.80 - width, SCREEN_HEIGHT * 0.6F, width, 300, 20, HEADER,
               KEY_B) {
    PLAYER_BAG = new InventorySlot[max_slots];
  }
//TODO add AddSlots 
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
      bagPanel.isOpen = false;
      return;
    }
    DRAG_WINDOW()
    DrawWindow();
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      PLAYER_BAG[i].Draw(wholeWindow.x, wholeWindow.y);
    }
    bagPanel.Draw(wholeWindow.x, wholeWindow.y);
  }
  void Update() noexcept {
    wholeWindow.height = 60 + std::max(PLAYER_BAG_SIZE % per_row + 1, 1) * spacing_y;
    WINDOW_UPDATE();
    for (uint_fast32_t i = 0; i < PLAYER_BAG_SIZE; i++) {
      PLAYER_BAG[i].Update();
    }
    bagPanel.Update();
  }
  inline static void AddSlots(int n) noexcept {
    //todo add dynamic inventory size based on slots
    int exist_x = PLAYER_BAG_SIZE % per_row;
    int exist_y = PLAYER_BAG_SIZE / per_row;
    for (uint_fast32_t i = 0; i < n; i++) {
      RectangleR rect = {(float)offset_x + spacing_x * exist_x,
                         (float)offset_y + spacing_y * exist_y, 45, 45};
      PLAYER_BAG[i].hitBox = rect;
      PLAYER_BAG[i].baseX = rect.x;
      PLAYER_BAG[i].baseY = rect.y;
      PLAYER_BAG_SIZE++;
      exist_x = PLAYER_BAG_SIZE % per_row;
      exist_y = PLAYER_BAG_SIZE / per_row;
    }
  }
  inline static void RemoveSlots(int n) noexcept {}
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
