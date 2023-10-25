#ifndef MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
struct BagPanel {
  inline static int BASE_BAG_SLOTS = 9;
  std::array<InventorySlot, 4> bagSlots = {
      InventorySlot(0, 2, ItemType::BAG, 25, 25),
      InventorySlot(0, 2, ItemType::BAG, 25, 25),
      InventorySlot(0, 2, ItemType::BAG, 25, 25),
      InventorySlot(0, 2, ItemType::BAG, 25, 25),
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
  SlideComponent slide{isOpen, 3 * 29, 3.5F};

  void Draw(float x, float y) noexcept {
    y += 25;
    if (bagPanelButton.Draw(x + 25, y)) {
      isOpen = !isOpen;
    }
    x += 45;
    if (IsOpened()) {
      int i = 0;
      bool once = false;
      for (auto& slot : bagSlots) {
        slot.Draw(x, y);
        i++;
        if (slide.GetCurrentOffset() > i * 28) {
          x += 28;
        } else if (!once) {
          x += (int)slide.GetCurrentOffset() % 28;
          once = true;
        }
      }
    }
  }
  void Update() noexcept {
    slide.UpdateAnimation();
    int size = BASE_BAG_SLOTS;
    for (auto& slot : bagSlots) {
      if (slot.item) {
        size += slot.item->effects[BAG_SLOTS];
      }
      slot.UpdateCharacterSlots();
    }

    int playerBagSlots = (int)PLAYER_STATS.GetBagSlots();
    if (playerBagSlots < size) {
      PLAYER_STATS.effects[BAG_SLOTS] += size - playerBagSlots;
    } else if (playerBagSlots > size) {
      RemoveSlots(playerBagSlots - size);
    }
  }
  [[nodiscard]] inline bool IsOpened() const noexcept { return isOpen || !slide.IsAnimationFinished(); }

 private:
  static inline void RemoveSlots(int n);
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_BAGPANEL_H_
