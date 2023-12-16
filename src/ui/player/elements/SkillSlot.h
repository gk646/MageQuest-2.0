#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLSLOT_H_

//A wrapper for a skill
struct SkillSlot {
  RectangleR hitBox = {0};
  Skill* skill;
  uint16_t baseX = 0, baseY = 0;
  int8_t toolTipHoverTicks = 0;
  bool withBackGround;
  SkillSlot(int x, int y, Skill* skill = SKILLS[LOCKED], bool withBackGround = true,
            float width = 50, float height = 50) noexcept
      : hitBox((uint16_t)x, (uint16_t)y, width, height),
        baseX(x),
        baseY(y),
        withBackGround(withBackGround),
        skill(skill) {}
  //Draws the icon
  inline bool Draw(float x, float y, int keyBind = 0) noexcept {
    hitBox.x = (x + (float)baseX);
    hitBox.y = (y + (float)baseY);

    if (keyBind != 0) {
      if (keyBind == -1) {
        DrawTextureProFast(textures::ui::skillbar::mouseleft,hitBox.x+4+25-6,hitBox.y+3+60,0,WHITE);
      } else if (keyBind == -2) {
        DrawTextureProFast(textures::ui::skillbar::mouseright,hitBox.x+4+25-6,hitBox.y+3+60,0,WHITE);
      }else{
        snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%s", Util::GetKeyName(keyBind));
        Util::DrawCenteredText(MINECRAFT_BOLD, 16, TEXT_BUFFER,
                               hitBox.x + 4 + hitBox.width / 2, hitBox.y + 3 + 60,
                               Colors::white_smoke);
      }
    }

    if (withBackGround) {
      DrawTextureProFast(textures::ui::skillbar::slot, hitBox.x, hitBox.y, 0, WHITE);
    }

    auto ptr = skill;  //thread safety
    if (ptr) {
      ptr->Draw(hitBox.x + 4, hitBox.y + 3);
      return toolTipHoverTicks < 0;
    }
  }
  inline void Update() noexcept {
    if (CheckCollisionPointRec(MOUSE_POS, hitBox)) {
      SkillSlotsExchangeLogic();
      toolTipHoverTicks = (int8_t)std::max(toolTipHoverTicks - 1, -1);
    } else {
      toolTipHoverTicks = 12;
    }

    if (skill) {
      skill->Update();
    }
  }
  //Draws the skill tooltip
  inline void DrawToolTip() const noexcept {
    if (skill) {
      skill->DrawTooltip(hitBox.x + 3, hitBox.y + 2);
    }
  }

 private:
  inline void SkillSlotsExchangeLogic() noexcept {
    if (!DRAGGED_SKILL_SLOT && IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
        skill != SKILLS[LOCKED]) {
      DRAGGED_SKILL_SLOT = this;
    } else if (DRAGGED_SKILL_SLOT && IsMouseButtonUp(MOUSE_BUTTON_LEFT) &&
               !withBackGround && skill != SKILLS[LOCKED]) {
      this->skill = DRAGGED_SKILL_SLOT->skill;
      DRAGGED_SKILL_SLOT = nullptr;
    }
  }
};

void PlayerStats::IncrementSkillLevels() noexcept {
  for (auto& skill : PLAYER_SKILLS) {
    skill->skill->skillStats.skillLevel++;
  }
}
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLSLOT_H_
