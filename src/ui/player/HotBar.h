#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "XPBar.h"

struct HotBar {
  inline static constexpr uint8_t BUTTON_ALPHA = 70;
  std::array<TexturedButton, 6> menuButtons{
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::bag,
                     textures::ui::skillbar::bag, textures::ui::skillbar::bag_pressed,
                     BUTTON_ALPHA, "Opens your bags (B)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::abilities,
                     textures::ui::skillbar::abilities,
                     textures::ui::skillbar::abilities_pressed, BUTTON_ALPHA,
                     "Opens abilities (P)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::character,
                     textures::ui::skillbar::character,
                     textures::ui::skillbar::character_pressed, BUTTON_ALPHA,
                     "Opens character (C)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::map,
                     textures::ui::skillbar::map, textures::ui::skillbar::map_pressed,
                     BUTTON_ALPHA, "Opens map (M)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::setting,
                     textures::ui::skillbar::setting,
                     textures::ui::skillbar::setting_pressed, BUTTON_ALPHA,
                     "Opens settings (ESC)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::skilltree,
                     textures::ui::skillbar::skilltree,
                     textures::ui::skillbar::skilltree_pressed, BUTTON_ALPHA,
                     "Opens skill tree (N)")};
  std::array<Texture, 6> icons{
      textures::EMPTY_TEXTURE,           textures::EMPTY_TEXTURE,
      textures::EMPTY_TEXTURE,           textures::EMPTY_TEXTURE,
      textures::ui::skillbar::mouseleft, textures::ui::skillbar::mouseright};
  std::array<Skill*, 6> skills{new Dummy_Skill(), new Dummy_Skill(), new Dummy_Skill(),
                               new Dummy_Skill(), new Dummy_Skill(), new Dummy_Skill()};
  XPBar xp_bar;
  RectangleR BASE_RECT = {0, 0, 480, 120};
  HotBar() noexcept = default;
  void Draw() noexcept {
    float offX = SCALE(52);
    float offY = SCALE(23);
    float width = SCALE(BASE_RECT.width);
    float height = SCALE(BASE_RECT.height);
    float startX = (SCREEN_WIDTH - width) / 2.0F;
    float startY = SCREEN_HEIGHT - height * 0.93F;
    xp_bar.Draw(startX + 5, startY);
    PLAYER_EFFECTS.DrawPlayer(startY - 44);
    DrawSkillIcons(startX, startY, width, height, offX, offY);
    DrawMenuButtons(startX + 650);
    Skill::DrawCastBar();
  }
  void Update() noexcept {
    xp_bar.Update();
    for (auto& mb : menuButtons) {
      mb.UpdateGlobalWindowState();
    }
    for (const auto& skill : skills) {
      skill->Update();
    }
    if (Skill::castProgress >= 0) {
      std::cout << Skill::castProgress << std::endl;
      Skill::castProgress++;
      if (PLAYER.moving) Skill::castProgress = -1;
      if (Skill::castProgress == Skill::lastCastedSkill->skillStats.castTime) {
        std::cout << "castd" << std::endl;
        Skill::lastCastedSkill->Activate(false);
      }
    }
    if (GAME_STATE != GameState::GameMenu) {
      if (IsKeyDown(KEY_ONE) && skills[0]->IsUsable()) {
        skills[0]->Activate(false);
      }
      if (IsKeyDown(KEY_TWO) && skills[1]->IsUsable()) {
        skills[1]->Activate(false);
      }
      if (IsKeyDown(KEY_THREE) && skills[2]->IsUsable()) {
        skills[2]->Activate(false);
      }
      if (IsKeyDown(KEY_FOUR) && skills[3]->IsUsable()) {
        skills[3]->Activate(false);
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && skills[4]->IsUsable() &&
          !WINDOW_FOCUSED && !DRAGGED_ITEM) {
        skills[4]->Activate(false);
      }
      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && skills[5]->IsUsable() &&
          !WINDOW_FOCUSED) {
        skills[5]->Activate(false);
      }
    }
  }

 private:
  //Draws the skill icons and appropriate tooltip
  inline void DrawSkillIcons(float startX, float startY, float width, float height,
                             float offX, float offY) noexcept {

    DrawTextureScaled(textures::ui::skillbar::skillbar, {startX, startY, width, height},
                      0, false, 0, WHITE);

    float currentX = startX;
    Skill* toolTipSkill = nullptr;
    int16_t j;
    for (int16_t i = 0; i < 6; i++) {
      if (skills[i]->Draw(currentX + offX, startY + offY)) {
        toolTipSkill = skills[i];
        j = i;
      }
      DrawTextureProFast(
          icons[i], currentX + offX + SCALE(50) / 2.0F - (float)icons[i].width / 2.0F,
          startY + height * 0.72F, 0, WHITE);
      currentX += SCALE(65);
    }
    if (toolTipSkill) {
      toolTipSkill->DrawTooltip(startX + (float)j * 65.0F + offX, startY + offY);
    }

  }
  //Draws semi transparent interface buttons on the lower right screen
  inline void DrawMenuButtons(float x) noexcept {
    for (int i = 5; i > -1; i--) {
      menuButtons[i].Draw(x, SCREEN_HEIGHT - menuButtons[i].bounds.height);
      x -= 32;
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
