#ifndef MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
struct BagPanel {
  inline static constexpr float BAG_PANEL_WIDTH = 80;
  inline static constexpr float BAG_PANEL_HEIGHT = 29;
  std::array<InventorySlot, 4> bagSlots = {
      InventorySlot(2, 2, ItemType::BAG, 25, 25),
      InventorySlot(2 + 26, 2, ItemType::BAG, 25, 25),
      InventorySlot(2 + 26 * 2, 2, ItemType::BAG, 25, 25),
      InventorySlot(2 + 26 * 3, 2, ItemType::BAG, 25, 25),
  };
  TexturedButton bagPanelButton = {25,
                                   25,
                                   "",
                                   15,
                                   textures::ui::questpanel::questBox,
                                   textures::ui::questpanel::questBox,
                                   textures::ui::questpanel::questBox,
                                   255,
                                   "Opens the bag slots"};
  bool isOpen = false;
  SlideComponent slide{isOpen, 35, 2};

  void Draw(float x, float y) noexcept {
    y += 25;
    if (bagPanelButton.Draw(x + 25, y)) {
      isOpen = !isOpen;
    }
    if (isOpen || !slide.IsAnimationFinished()) {
      x += slide.GetCurrentOffset();
      for (auto& slot : bagSlots) {
        slot.Draw(x, y);
      }
    }
  }

  void Update() noexcept {
    slide.UpdateAnimation();
    int size = PLAYER_BAG_SIZE;
    for (auto& slot : bagSlots) {
      if (slot.item) {
        size += slot.item->effects[BAG_SLOTS];
      }
      slot.Update();
    }
    if (PLAYER_BAG_SIZE != size) {
      CharacterBag::AddSlots()
    }
  }

 private:
  inline void HandleAnimation() noexcept {}
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
