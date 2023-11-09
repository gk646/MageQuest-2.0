#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TALENTNODE_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TALENTNODE_H_

#include "../../../gameplay/Talent.h"

struct TalentNode {
  inline static constexpr float TOOLTIP_WIDTH = 230;
  inline static constexpr float TOOLTIP_HEIGHT = 130;
  Talent talent;
  SoundComponent soundPlayer;
  RectangleR bounds{};
  Point basePoint{0, 0};
  int16_t talentID = -1;
  bool isActivated = false;
  bool isUsable = false;
  TalentSize sizeType = TalentSize::NORMAL;
  bool isHovered = false;
  explicit TalentNode(const Point& p, TalentSize sizeType, int16_t nodeID)
      : sizeType(sizeType), basePoint(p), talentID(nodeID) {
    if (sizeType == TalentSize::NORMAL) {
      this->bounds = {944 + p.x(), 524 + p.y(), 32, 32};
    } else if (sizeType == TalentSize::MID) {
      this->bounds = {938 + p.x(), 518 + p.y(), 45, 45};
    } else if (sizeType == TalentSize::BIG) {
      this->bounds = {935 + +p.x(), 515 + p.y(), 50, 50};
    }
  }
  inline void Draw(const Texture& tex, float offX, float offY,
                   int16_t& toolTipID) noexcept {
    UpdateImpl(tex, offX, offY);
    DrawTextureProFast(tex, bounds.x, bounds.y, 0, WHITE);
    DrawTalentIcon();
    SpendTalentPoint();
    if (isHovered) {
      toolTipID = talentID;
    }
  }
  inline void Update() noexcept { isHovered = CheckCollisionPointRec(MOUSE_POS, bounds); }
  //Draws the tooltip with the given bounds
  inline void DrawToolTip(const RectangleR& toolTipBounds) const noexcept {
    DrawRectangleRounded(toolTipBounds, 0.1F, ROUND_SEGMENTS,
                         Colors::mediumLightGreyTransparent);
    DrawRectangleRoundedLines(toolTipBounds, 0.1F, ROUND_SEGMENTS, 2,
                              Colors::darkBackground);

    DrawTextExR(MINECRAFT_BOLD, talent.name.c_str(),
                {toolTipBounds.x + 5, toolTipBounds.y + 5}, 18, 0.5F,
                Colors::darkBackground);
    Util::DrawWrappedText({toolTipBounds.x + 5, toolTipBounds.y + 25}, talent.description,
                          TOOLTIP_WIDTH, MINECRAFT_REGULAR, 15, Colors::darkBackground);
  }

 private:
  //Updates the nodes internal values // called on the draw thread
  inline void UpdateImpl(const Texture& tex, float offX, float offY) noexcept {
    bounds.x = basePoint.x() + offX;
    bounds.y = basePoint.y() + offY;
    isUsable = &tex == &textures::ui::talentpanel::TALENT_BIG_PURPLE ||
               &tex == &textures::ui::talentpanel::NODE_PURPLE ||
               &tex == &textures::ui::talentpanel::TALENT_MID_PURPLE;
  }
  //Draws the talent icon with the correct offset
  inline void DrawTalentIcon() const noexcept {
    if (sizeType == TalentSize::BIG) {
      DrawTextureProFast(talent.icon, bounds.x + 8, bounds.y + 8, 0, WHITE);
    } else if (sizeType == TalentSize::MID) {
      DrawTextureProFast(talent.icon, bounds.x + 8, bounds.y + 8, 0, WHITE);
    }else{
      DrawTextureProFast(talent.icon, bounds.x + 8, bounds.y + 8, 0, WHITE);
    }
  }
  //Spends a talent point on this node
  inline void SpendTalentPoint() noexcept {
    if (isUsable && !isActivated && isHovered &&
        IsMouseButtonReleased(MOUSE_BUTTON_LEFT) &&
        PLAYER_SPENT_POINTS.SpendTalentPoint()) {
      DataBaseHandler::AddActivatedTalent(talentID);
      PLAYER_STATS.EquipItem(talent.effects);
      if (talent.talentEffect) {
        TALENT_EFFECTS.push_back(talent.talentEffect);
      }
      isActivated = true;
      soundPlayer.PlaySound(&sound::talents::spendTalentPoint);
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TALENTNODE_H_
