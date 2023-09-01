#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

struct HotBar {

  int columns = 5;
  int rows = 1;


  HotBar(int columns, int rows) : columns(columns), rows(rows) {}
  void draw() const noexcept {
    const float slotSize = 30 * UI_SCALE;    // Size of each slot
    const float slotSpacing = 5 * UI_SCALE;  // Spacing between slots

    const float hotbarWidth = columns * slotSize + (columns - 1) * slotSpacing;
    const float hotbarHeight = rows * slotSize + (rows - 1) * slotSpacing;

    const float startX = (SCREEN_WIDTH - hotbarWidth) / 2.0;
    const float startY = SCREEN_HEIGHT - (SCREEN_HEIGHT - hotbarHeight) / 8.0;

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < columns; col++) {
        float slotX = startX + col * (slotSize + slotSpacing);
        float slotY = startY + row * (slotSize + slotSpacing);
        DrawRectangleLinesEx({slotX, slotY, slotSize, slotSize}, 3, GRAY);
      }
    }
  }
  void update() noexcept {

  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
