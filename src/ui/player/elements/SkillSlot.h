#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLSLOT_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLSLOT_H_

//A wrapper for a skill
struct SkillSlot {
  RectangleR hitBox = {0};
  Skill* skill = SKILLS[LOCKED];
  uint16_t baseX = 0, baseY = 0;
  DamageType dmgType = DamageType::TRUE_DMG;
  int8_t toolTipHoverTicks = 0;
  SkillSlot() = default;
  SkillSlot(int x, int y, DamageType dmgType, float width = 50,
            float height = 50) noexcept
      : hitBox((uint16_t)x, (uint16_t)y, width, height),
        baseX(x),
        baseY(y),
        dmgType(dmgType) {}
  inline bool Draw(float x, float y) noexcept {
    hitBox.x = (x + (float)baseX);
    hitBox.y = (y + (float)baseY);

    auto ptr = skill;  //thread safety
    //DrawTextureProFast(textures::ui::skillbar::slot, hitBox.x, hitBox.y, 0, WHITE);
    if (ptr) {
      ptr->Draw(hitBox.x + 3, hitBox.y + 2);
      return toolTipHoverTicks < 0;
    }
  }
  inline void Update() noexcept {
    if (CheckCollisionPointRec(MOUSE_POS, hitBox)) {
      toolTipHoverTicks = std::max(toolTipHoverTicks - 1, -1);
    } else {
      toolTipHoverTicks = 12;
    }
  }
};

#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLSLOT_H_
