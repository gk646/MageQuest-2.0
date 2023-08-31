#ifndef DND_SRC_UI_HOTBAR_H_
#define DND_SRC_UI_HOTBAR_H_

#include "../Component.h"

class Hotbar : public Component {
 public:
  int columns = 10;
  int rows = 2;

  Hotbar(int columns, int rows) : columns(columns), rows(rows) {}
  void draw(float ui_scale, int screen_width, int screen_height) final {

    // Define the size and spacing of the hotbar slots.
    const float slotSize = 35 * ui_scale;     // Size of each slot
    const float slotSpacing = 8 * ui_scale;  // Spacing between slots

    // Calculate the total size of the hotbar.
    const float hotbarWidth = columns * slotSize + (columns - 1) * slotSpacing;
    const float hotbarHeight = rows * slotSize + (rows - 1) * slotSpacing;

    // Calculate starting position for the hotbar (centered on the screen).
    const float startX = (screen_width - hotbarWidth) / 2.0;
    const float startY = screen_height - (screen_height - hotbarHeight) / 8.0;

    // Draw the slots.
    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < columns; col++) {
        float slotX = startX + col * (slotSize + slotSpacing);
        float slotY = startY + row * (slotSize + slotSpacing);

        // Here we're just drawing a simple rectangle for each slot, but you can
        // replace this with a more sophisticated rendering if desired.
        DrawRectangleLines(slotX, slotY, slotSize, slotSize, WHITE);
      }
    }
  }
  void update() final{

  }
};

#endif  //DND_SRC_UI_HOTBAR_H_
