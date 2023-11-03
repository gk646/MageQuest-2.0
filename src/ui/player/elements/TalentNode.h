#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TALENTNODE_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TALENTNODE_H_

#include "../../../gameplay/Talent.h"
struct TalentNode {
  Talent talent;
  RectangleR bounds{};
  Point basePoint{0, 0};
  int16_t nodeID = -1;
  bool isActivated = false;
  TalentSize sizeType = TalentSize::NORMAL;
  bool isHovered = false;
  explicit TalentNode(const Point& p, TalentSize sizeType, int16_t nodeID)
      : sizeType(sizeType), basePoint(p), nodeID(nodeID) {
    if (sizeType == TalentSize::NORMAL) {
      this->bounds = {944 + p.x(), 524 + p.y(), 32, 32};
    } else if (sizeType == TalentSize::MID) {
      this->bounds = {938 + p.x(), 518 + p.y(), 45, 45};
    } else if (sizeType == TalentSize::BIG) {
      this->bounds = {935 + +p.x(), 515 + p.y(), 50, 50};
    }
  }
  inline void Draw(const Texture& tex, float offX, float offY) noexcept {
    bounds.x = basePoint.x() + offX;
    bounds.y = basePoint.y() + offY;
    DrawTextureProFast(tex, bounds.x, bounds.y, 0, WHITE);
    if (sizeType == TalentSize::NORMAL) {
      DrawTextureProFast(talent.icon, bounds.x + 8, bounds.y + 8, 0, WHITE);
    } else if (sizeType == TalentSize::BIG) {
      DrawTextureProFast(talent.icon, bounds.x + 6, bounds.y + 6, 0, WHITE);
    } else {
      DrawTextureProFast(talent.icon, bounds.x + 8, bounds.y + 8, 0, WHITE);
    }
    SpendTalentPoint();
  }
  inline void Update() noexcept { isHovered = CheckCollisionPointRec(MOUSE_POS, bounds); }

 private:
  inline void SpendTalentPoint() noexcept {
    if (!isActivated && isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      isActivated = true;
     //TODO sound
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TALENTNODE_H_
