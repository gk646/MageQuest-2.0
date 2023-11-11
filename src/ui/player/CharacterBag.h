
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
  static constexpr int max_slots = per_row * 9;
  BagPanel bagPanel;
  explicit CharacterBag() noexcept
      : Window(SCREEN_WIDTH * 0.80 - width, SCREEN_HEIGHT * 0.6F, width, 300, 20, "Bags",
               KEY_B, sound::openBags, sound::closeBags) {
    PLAYER_BAG = new InventorySlot[max_slots];
    CalculateSlots(max_slots);
    PLAYER_STATS.effects[BAG_SLOTS] = BagPanel::BASE_BAG_SLOTS;
  }
  void Draw() noexcept {
    if (!isWindowOpen) {
      bagPanel.isOpen = false;
      bagPanel.slide.Reset();
    }
    WINDOW_LOGIC()
    DrawWindow();
    for (uint_fast32_t i = 0; i < (int)PLAYER_STATS.effects[BAG_SLOTS]; i++) {
      PLAYER_BAG[i].Draw(wholeWindow.x, wholeWindow.y);
    }
    bagPanel.Draw(wholeWindow.x, wholeWindow.y);
  }
  void Update() noexcept {
    float bagSlots = PLAYER_STATS.GetBagSlots();
    wholeWindow.height = 60 + std::ceil(bagSlots / per_row) * spacing_y;
    WINDOW_UPDATE()
    for (uint_fast32_t i = 0; i < (int)bagSlots; i++) {
      PLAYER_BAG[i].Update();
    }
    if (bagPanel.IsOpened()) {
      bagPanel.Update();
    }
  }
  inline static void CalculateSlots(int n) noexcept {
    int var = 0;
    int exist_x = var % per_row;
    int exist_y = var / per_row;
    for (uint_fast32_t i = 0; i < n; i++) {
      RectangleR rect = {(float)offset_x + spacing_x * exist_x,
                         (float)offset_y + spacing_y * exist_y, 45, 45};
      PLAYER_BAG[i].hitBox = rect;
      PLAYER_BAG[i].baseX = rect.x;
      PLAYER_BAG[i].baseY = rect.y;
      var++;
      exist_x = var % per_row;
      exist_y = var / per_row;
    }
  }
  inline static void RemoveSlots(int n) noexcept;
  static bool AddItem(Item* new_item) noexcept {
    for (uint_fast32_t i = 0; i < PLAYER_STATS.GetBagSlots(); i++) {
      if (!PLAYER_BAG[i].item && &PLAYER_BAG[i] != DRAGGED_SLOT) {
        PLAYER_BAG[i].item = new_item;
        GAME_STATISTICS.PickedUpItem(new_item->rarity);
        return true;
      }
    }
    return false;
  }
};

void EntityStats::UnEquipItem(const float* effect_arr) noexcept {
  for (uint_fast32_t i = 0; i < STATS_ENDING; i++) {
    effects[i] -= effect_arr[i];
  }
  CharacterBag::RemoveSlots(effect_arr[BAG_SLOTS]);
  ReCalculatePlayerStats();
}

#endif  //MAGEQUEST_SRC_UI_PLAYER_CHARACTERBAG_H_
