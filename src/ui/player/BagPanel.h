#ifndef MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
struct BagPanel {
  inline static int BASE_BAG_SLOTS = 9;
  std::array<ItemSlot, 4> bagSlots = {
      ItemSlot(0, 2, ItemType::BAG, 25, 25),
      ItemSlot(0, 2, ItemType::BAG, 25, 25),
      ItemSlot(0, 2, ItemType::BAG, 25, 25),
      ItemSlot(0, 2, ItemType::BAG, 25, 25),
  };
  TexturedButton bagPanelButton = {25,
                                   25,
                                   "",
                                   15,
                                   textures::ui::bagSlots,
                                   textures::ui::bagSlotsHovered,
                                   textures::ui::bagSlotsPressed,
                                   255,
                                   "Opens the bag slots"};
  bool isOpen = false;
  SlideComponent slide{isOpen, 3 * 31, 3.5F};

  void Draw(float x, float y) noexcept {
    DrawBagPanelButton(x, y);
    if (IsOpened()) {
      DrawBagSlots(x + 43, y - 2);
    }
  }
  void Update() noexcept {
    slide.UpdateAnimation();
    for (auto& slot : bagSlots) {
      slot.UpdateCharacterSlots();
    }
  }
  [[nodiscard]] inline bool IsOpened() const noexcept {
    return isOpen || !slide.IsAnimationFinished();
  }

 private:
  inline void DrawBagPanelButton(float x, float y) noexcept {
    if (bagPanelButton.Draw(x + 25, y)) {
      isOpen = !isOpen;
    }
  }
  inline void DrawBagSlots(float x, float y) noexcept {
    int i = 0;
    bool once = false;
    for (auto& slot : bagSlots) {
      slot.Draw(x, y);
      i++;
      if (slide.GetCurrentOffset() > i * 30) {
        x += 30;
      } else if (!once) {
        x += static_cast<int>(slide.GetCurrentOffset()) % 30;
        once = true;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
