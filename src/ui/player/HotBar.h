#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "../../gameplay/Skill.h"
#include "elements/XPBar.h"

struct HotBar {

  RectangleR BASE_RECT = {0, 0, 480, 120};
  XPBar xp_bar;
  std::array<Skill*, 6> skills{new Dummy_Skill(), new Dummy_Skill(), new Dummy_Skill(),
                               new Dummy_Skill(), new Dummy_Skill(), new Dummy_Skill()};
  std::array<Texture*, 6> icons{new Texture(),
                                new Texture(),
                                new Texture(),
                                new Texture(),
                                &textures::ui::skillbar::mouseleft,
                                &textures::ui::skillbar::mouseright};
  HotBar() noexcept = default;
  void draw() noexcept {
    float size = SCALE(50);
    float offx = SCALE(46);
    float offy = SCALE(23);
    float width = SCALE(BASE_RECT.width);
    float height = SCALE(BASE_RECT.height);
    float dx = (SCREEN_WIDTH - width) / 2.0F;
    float dy = SCREEN_HEIGHT - height * 0.93;
    xp_bar.draw(dx, dy);
    DrawTexturePro(textures::ui::skillbar::skillbar, BASE_RECT, {dx, dy, width, height},
                   {0, 0}, 0, WHITE);
    for (uint_fast32_t i = 0; i < 6; i++) {
      skills[i]->draw(dx + offx, dy + offy, size);
      DrawTextureProFast(*icons[i], dx + offx + SCALE(50) / 2 - icons[i]->width / 2,
                         dy + height * 0.72, 0, WHITE);
      dx += SCALE(65);
    }
  }
  void update() noexcept {
    xp_bar.update();
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
