#ifndef MAGE_QUEST_SRC_UI_HOTBAR_H_
#define MAGE_QUEST_SRC_UI_HOTBAR_H_

#include "XPBar.h"

struct HotBar {
  inline static constexpr uint8_t BUTTON_ALPHA = 70;
  std::array<TexturedButton, 7> menuButtons{
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::character,
                     textures::ui::skillbar::character,
                     textures::ui::skillbar::character_pressed, BUTTON_ALPHA,
                     "Opens character (C)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::bag,
                     textures::ui::skillbar::bag, textures::ui::skillbar::bag_pressed,
                     BUTTON_ALPHA, "Opens your bags (B)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::journal,
                     textures::ui::skillbar::journal,
                     textures::ui::skillbar::journal_pressed, BUTTON_ALPHA,
                     "Opens journal (J)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::abilities,
                     textures::ui::skillbar::abilities,
                     textures::ui::skillbar::abilities_pressed, BUTTON_ALPHA,
                     "Opens abilities (P)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::skilltree,
                     textures::ui::skillbar::skilltree,
                     textures::ui::skillbar::skilltree_pressed, BUTTON_ALPHA,
                     "Opens skill tree (N)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::map,
                     textures::ui::skillbar::map, textures::ui::skillbar::map_pressed,
                     BUTTON_ALPHA, "Opens map (M)"),
      TexturedButton(32, 32, "", 16, textures::ui::skillbar::setting,
                     textures::ui::skillbar::setting,
                     textures::ui::skillbar::setting_pressed, BUTTON_ALPHA,
                     "Opens settings (ESC)"),
  };
  XPBar experienceBar;
  RectangleR hotBarHitbox = {0, 0, 480, 120};
  HotBar() = default;

 public:
  void Draw() noexcept {
    Skill::DrawCastBar();
    float startX = (SCREEN_WIDTH - hotBarHitbox.width) / 2.0F;
    float startY = SCREEN_HEIGHT - hotBarHitbox.height * 0.93F;

    experienceBar.Draw(startX + 5, startY);
    PLAYER_EFFECTS.DrawPlayer(startY - 44);
    DrawMenuButtons(startX + 850);

    DrawHotbar(startX, startY);
    UpdatePlayerSkills();
    if (IsKeyPressed(PLAYER_KEYBINDS[(int)Keybind::PLAYER_LIGHT])) {
      Lighting::Shaders::lightOn = !Lighting::Shaders::lightOn;
    }
  }
  void Update() noexcept {
    Skill::UpdateStaticState();
    experienceBar.Update();
    for (auto& mb : menuButtons) {
      mb.UpdateGlobalWindowState();
    }
    for (auto& slot : PLAYER_HOTBAR) {
      slot->Update();
    }
  }

 private:
  //Called on the main thread // if button is pressed activates the skill
  inline static void UpdatePlayerSkills() noexcept {
    if (GAME_STATE != GameState::GameMenu) {
      if (IsKeyPressed(KEY_ONE) && PLAYER_HOTBAR[0]->skill->IsUsable()) {
        PLAYER_HOTBAR[0]->skill->Activate(false);
      }
      if (IsKeyPressed(KEY_TWO) && PLAYER_HOTBAR[1]->skill->IsUsable()) {
        PLAYER_HOTBAR[1]->skill->Activate(false);
      }
      if (IsKeyPressed(KEY_THREE) && PLAYER_HOTBAR[2]->skill->IsUsable()) {
        PLAYER_HOTBAR[2]->skill->Activate(false);
      }
      if (IsKeyPressed(KEY_FOUR) && PLAYER_HOTBAR[3]->skill->IsUsable()) {
        PLAYER_HOTBAR[3]->skill->Activate(false);
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
          PLAYER_HOTBAR[4]->skill->IsUsable() && !WINDOW_FOCUSED && !DRAGGED_ITEM) {
        PLAYER_HOTBAR[4]->skill->Activate(false);
      }
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
          PLAYER_HOTBAR[5]->skill->IsUsable() && !WINDOW_FOCUSED) {
        PLAYER_HOTBAR[5]->skill->Activate(false);
      }
    }
  }
  //Draws the skill icons and appropriate tooltip
  inline void DrawHotbar(float startX, float startY) noexcept {
    DrawTextureScaled(textures::ui::skillbar::skillbar,
                      {startX, startY, hotBarHitbox.width, hotBarHitbox.height}, 0, false,
                      0, WHITE);
    SkillSlot* toolTip = nullptr;
    for (auto& slot : PLAYER_HOTBAR) {
      if (slot->Draw(startX, startY)) {
        toolTip = slot;
      }
    }
    if (toolTip) {
      toolTip->DrawToolTip();
    }
  }
  //Draws semi transparent interface buttons on the lower right screen
  inline void DrawMenuButtons(float x) noexcept {
    for (int i = 6; i > -1; i--) {
      menuButtons[i].Draw(x, SCREEN_HEIGHT - menuButtons[i].bounds.height);
      x -= 32;
    }
  }
};

#endif  //MAGE_QUEST_SRC_UI_HOTBAR_H_
