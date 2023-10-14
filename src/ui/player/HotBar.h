#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "../../gameplay/Skill.h"
#include "XPBar.h"
#include "../elements/TexturedButton.h"

struct HotBar {
  inline static constexpr uint8_t BUTTON_ALPHA = 70;
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
  std::array<TexturedButton, 6> menuButtons{
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::bag, textures::ui::skillbar::bag,
             textures::ui::skillbar::bag_pressed, BUTTON_ALPHA, "Opens your bags (B)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::abilities, textures::ui::skillbar::abilities,
             textures::ui::skillbar::abilities_pressed, BUTTON_ALPHA, "Opens abilities (P)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::character, textures::ui::skillbar::character,
             textures::ui::skillbar::character_pressed, BUTTON_ALPHA, "Opens character (C)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::map, textures::ui::skillbar::map,
             textures::ui::skillbar::map_pressed, BUTTON_ALPHA, "Opens map (M)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::setting, textures::ui::skillbar::setting,
             textures::ui::skillbar::setting_pressed, BUTTON_ALPHA, "Opens settings (ESC)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::skilltree, textures::ui::skillbar::skilltree,
             textures::ui::skillbar::skilltree_pressed, BUTTON_ALPHA, "Opens skill tree (N)")
  };
  HotBar() noexcept = default;
  void Draw() noexcept {
    float size = SCALE(50);
    float offx = SCALE(46);
    float offy = SCALE(23);
    float width = SCALE(BASE_RECT.width);
    float height = SCALE(BASE_RECT.height);
    float dx = (SCREEN_WIDTH - width) / 2.0F;
    float dy = SCREEN_HEIGHT - height * 0.93;
    xp_bar.Draw(dx, dy);
    DrawTexturePro(textures::ui::skillbar::skillbar, BASE_RECT, {dx, dy, width, height},
                   {0, 0}, 0, WHITE);
    for (uint_fast32_t i = 0; i < 6; i++) {
      skills[i]->Draw(dx + offx, dy + offy, size);
      DrawTextureProFast(*icons[i], dx + offx + SCALE(50) / 2 - icons[i]->width / 2,
                         dy + height * 0.72, 0, WHITE);
      dx += SCALE(65);
    }
    dx += 450;
    for (int i = 5; i > -1; i--) {
      menuButtons[i].Draw(dx, SCREEN_HEIGHT -  menuButtons[i].bounds.height);
      dx -= 32;
    }

  }
  void Update() noexcept {
    xp_bar.update();
    for (const auto& mb : menuButtons) {
      mb.UpdateGlobalWindowState();
    }
    for (const auto& skill : skills) {
      skill->Update();
    }
    if (GAME_STATE != GameState::GameMenu) {
      if (IsKeyDown(KEY_ONE) && skills[0]->IsUsable()) {
        skills[0]->activate();
      }
      if (IsKeyDown(KEY_TWO) && skills[1]->IsUsable()) {
        skills[1]->activate();
      }
      if (IsKeyDown(KEY_THREE) && skills[2]->IsUsable()) {
        skills[2]->activate();
      }
      if (IsKeyDown(KEY_FOUR) && skills[3]->IsUsable()) {
        skills[3]->activate();
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && skills[4]->IsUsable() &&
          !WINDOW_FOCUSED && !DRAGGED_ITEM) {
        skills[4]->activate();
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && skills[5]->IsUsable() &&
          !WINDOW_FOCUSED) {
        skills[5]->activate();
      }
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
