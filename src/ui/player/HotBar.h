#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "../../gameplay/Skill.h"

struct HotBar {
  int columns = 5;
  int rows = 1;
  std::array<Skill*, 5> skills{};
  HotBar(int columns, int rows) : columns(columns), rows(rows) {}
  void draw() const noexcept {
    const float slotSize = 30 * UI_SCALE;
    const float slotSpacing = 5 * UI_SCALE;

    const float hotbarWidth = columns * slotSize + (columns - 1) * slotSpacing;
    const float hotbarHeight = rows * slotSize + (rows - 1) * slotSpacing;

    const float startX = (SCREEN_WIDTH - hotbarWidth) / 2.0F;
    const float startY = SCREEN_HEIGHT - (SCREEN_HEIGHT - hotbarHeight) / 8.0F;

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < columns; col++) {
        float slotX = startX + col * (slotSize + slotSpacing);
        float slotY = startY + row * (slotSize + slotSpacing);
        skills[columns]->draw(slotX, slotY, slotSize);
      }
    }
  }
  void update() noexcept {
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
    if (IsKeyDown(KEY_FIVE) && skills[4]->use_able()) {
      skills[4]->activate();
    }

    for (const auto& skill : skills) {
      if (skill) {
        skill->update();
      }
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
