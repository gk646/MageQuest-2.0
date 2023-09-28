#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TEXTCELL_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TEXTCELL_H_
struct TextCell {
  RectangleR bounds;
  float base_width, base_height;
  std::string toolTip;
  const Font& font;
  float fontSize;
  bool isHovered = false;
  TextCell(float width, float height, std::string tooltip, const Font& font,
           float fontSize)
      : bounds(0, 0, width, height),
        toolTip(std::move(tooltip)),
        font(font),
        fontSize(fontSize),
        base_width(width),
        base_height(height) {}

  inline void DrawStatCell(float x, float y, char* txt, float rightVal,
                           const Color& rightColor = Colors::darkBackground,
                           const Color& leftColor = Colors::darkBackground) noexcept {
    Update(x, y);

    if (isHovered) {
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1,
                  Colors::mediumLightGreyDarker);
    } else {
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1, leftColor);
    }

    sprintf(txt, "%.1f", rightVal);
    DrawRightAlignedText(MINECRAFT_BOLD, SCALE(fontSize), txt, x + bounds.width, y,
                         rightColor);
    DrawToolTip();
  }
  inline void DrawStatCell(float x, float y, char* txt, int rightVal,
                           const Color& rightColor = Colors::darkBackground,
                           const Color& leftColor = Colors::darkBackground) noexcept {
    Update(x, y);

    if (isHovered) {
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1,
                  Colors::mediumLightGreyDarker);
    } else {
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1, leftColor);
    }

    sprintf(txt, "%i", (int)rightVal);
    DrawRightAlignedText(MINECRAFT_BOLD, SCALE(fontSize), txt, x + bounds.width, y,
                         rightColor);
    DrawToolTip();
  }
  static std::array<TextCell, 18> CreateCharacterCells(float width, float height,
                                                       const Font& font,
                                                       float fontSize) noexcept {
    auto toolTips = loadStringsFromFile(ASSET_PATH + "Text/Codex/StatsToolTip.txt");
    std::array<TextCell, 18> cells{
        TextCell(width, height, toolTips[0], font, fontSize),
        TextCell(width, height, toolTips[1], font, fontSize),
        TextCell(width, height, toolTips[2], font, fontSize),
        TextCell(width, height, toolTips[3], font, fontSize),
        TextCell(width, height, toolTips[4], font, fontSize),
        TextCell(width, height, toolTips[5], font, fontSize),
        TextCell(width, height, toolTips[6], font, fontSize),
        TextCell(width, height, toolTips[7], font, fontSize),
        TextCell(width, height, toolTips[8], font, fontSize),

        TextCell(width+25, height, toolTips[9], font, fontSize),
        TextCell(width+25, height, toolTips[10], font, fontSize),
        TextCell(width+25, height, toolTips[11], font, fontSize),
        TextCell(width+25, height, toolTips[12], font, fontSize),
        TextCell(width+25, height, toolTips[13], font, fontSize),
        TextCell(width+25, height, toolTips[14], font, fontSize),
        TextCell(width+25, height, toolTips[15], font, fontSize),
        TextCell(width+25, height, toolTips[16], font, fontSize),
        TextCell(width+25, height, toolTips[17], font, fontSize),
    };
    return cells;
  }

 private:
  inline void DrawToolTip() noexcept {
    isHovered = CheckCollisionPointRec(MOUSE_POS, bounds);
    Vector2 txtBounds = MeasureTextEx(font, toolTip.c_str(), SCALE(fontSize), 1);
    if (isHovered) {
      DrawTextureScaled(textures::ui::toolTip,
                        {MOUSE_POS.x, MOUSE_POS.y - bounds.height * 1.5F,
                         txtBounds.x + 25, txtBounds.y + 5},
                        0, false, 0, WHITE);
      DrawTextExR(font, toolTip.c_str(),
                  {MOUSE_POS.x + 12, MOUSE_POS.y - bounds.height * 1.3F}, SCALE(fontSize),
                  1, Colors::mediumVeryLight);
    }
  }
  inline void Update(float x, float y) noexcept {
    bounds.x = x;
    bounds.y = y;
    bounds.width = SCALE(base_width);
    bounds.height = SCALE(base_height);
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TEXTCELL_H_
