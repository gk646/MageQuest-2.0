#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

class HotBar {
 public:
  int columns = 10;
  int rows = 2;

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

        DrawRectangleLines(slotX, slotY, slotSize, slotSize, GRAY);
      }
    }
  }
  void update() noexcept {}
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
