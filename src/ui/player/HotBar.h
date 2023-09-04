#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "../../gameplay/Skill.h"

struct HotBar {
  int columns = 6;
  int rows = 1;
  std::array<Skill*, 6> skills{new Dummy(), new Dummy(), new Dummy(),
                               new Dummy(), new Dummy(), new Dummy()};
  HotBar(int columns, int rows) : columns(columns), rows(rows) {}
  void draw() const noexcept {
    if (IsKeyDown(KEY_ONE) && skills[0]->use_able()) {
      skills[0]->activate();
    }
    if (IsKeyDown(KEY_TWO) && skills[1]->use_able()) {
      skills[1]->activate();
    }
    if (IsKeyDown(KEY_THREE) && skills[2]->use_able()) {
      skills[2]->activate();
    }
    if (IsKeyDown(KEY_FOUR) && skills[3]->use_able()) {
      skills[3]->activate();
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && skills[4]->use_able()) {
      skills[4]->activate();
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && skills[5]->use_able()) {
      skills[5]->activate();
    }

    const float slotSize = 30 * UI_SCALE;
    const float slotSpacing = 5 * UI_SCALE;

    const float hotbarWidth = columns * (slotSize + slotSpacing) - slotSpacing;
    const float hotbarHeight = rows * (slotSize + slotSpacing) - slotSpacing;

    const float startX = (SCREEN_WIDTH - hotbarWidth) / 2.0F;
    const float startY = SCREEN_HEIGHT - (SCREEN_HEIGHT - hotbarHeight) / 8.0F;

    for (int col = 0; col < columns; ++col) {
      const auto slotX = startX + col * (slotSize + slotSpacing);
      const auto slotY = startY;

      skills[col]->draw(slotX, slotY, slotSize);
    }
  }

  void update() noexcept {
    for (const auto& skill : skills) {
      skill->update();
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
