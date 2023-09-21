#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "../../gameplay/Skill.h"

struct HotBar {
  static constexpr int SKILL_SIZE = 64;
  std::array<Skill*, 6> skills{new Dummy_Skill(), new Dummy_Skill(), new Dummy_Skill(),
                               new Dummy_Skill(), new Dummy_Skill(), new Dummy_Skill()};
  std::array<Texture*, 6> icons{new Texture(),
                                new Texture(),
                                new Texture(),
                                new Texture(),
                                &textures::ui::skillbar::mouseleft,
                                &textures::ui::skillbar::mouseright};
  HotBar() noexcept {}

  void draw() noexcept {
    float dx = (SCREEN_WIDTH - 6 * (69) - 5) / 2.0F;
    float dy = SCREEN_HEIGHT * 0.9;
    for (uint_fast32_t i = 0; i < 6; i++) {
      skills[i]->draw(dx, dy, 50);
      DrawTextureProFast(*icons[i], dx + 32 - icons[i]->width / 2, dy + 60, 0, WHITE);
      dx += 69;
    }
  }
  void update() noexcept {
    for (const auto& skill : skills) {
      skill->update();
    }
    if (GAME_STATE != GameState::GameMenu) {
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
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && skills[4]->use_able() &&
          !WINDOW_FOCUSED && !DRAGGED_ITEM) {
        skills[4]->activate();
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && skills[5]->use_able() &&
          !WINDOW_FOCUSED) {
        skills[5]->activate();
      }
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
