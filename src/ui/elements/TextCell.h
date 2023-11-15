#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TEXTCELL_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TEXTCELL_H_

struct TextCell {
  std::string toolTip;
  RectangleR bounds;
  const Font& font;
  uint16_t baseWidth, baseHeight;
  uint8_t fontSize;
  bool isHovered = false;
  TextCell(float width, float height, std::string tooltip, const Font& font,
           float fontSize)
      : bounds(0, 0, width, height),
        toolTip(std::move(tooltip)),
        font(font),
        fontSize((uint8_t)fontSize),
        baseWidth((uint16_t)width),
        baseHeight((uint16_t)height) {}
  inline void DrawStatCell(float x, float y, const char* txt, float rightVal,
                           Alignment align = Alignment::RIGHT,
                           const Color& rightColor = Colors::darkBackground,
                           const Color& leftColor = Colors::darkBackground, const char* format = nullptr) noexcept {
    Update(x, y);
    if (isHovered) {
      ToolTip::DrawToolTipAtMouse(toolTip, font, fontSize);
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1,
                  Colors::mediumLightGreyDarker);
    } else {
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1, leftColor);
    }
    if(format == nullptr){
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, format, rightVal);
    }else{
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.1f", rightVal);
    }
    if (align == Alignment::LEFT) {
      DrawTextExR(MINECRAFT_BOLD, TEXT_BUFFER,
                  {x + MeasureTextEx(MINECRAFT_BOLD, TEXT_BUFFER, 15, 0.5F).x + 5, y},
                  SCALE(fontSize), 0.5F, rightColor);
    } else if (align == Alignment::RIGHT) {
      Util::DrawRightAlignedText(MINECRAFT_BOLD, SCALE(fontSize), TEXT_BUFFER,
                                 x + bounds.width, y, rightColor);
    }
  }
  inline void DrawStatCell(float x, float y, const char* txt, int rightVal,
                           const Color& rightColor = Colors::darkBackground,
                           const Color& leftColor = Colors::darkBackground) noexcept {
    Update(x, y);
    if (isHovered) {
      ToolTip::DrawToolTipAtMouse(toolTip, font, fontSize);
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1,
                  Colors::mediumLightGreyDarker);
    } else {
      DrawTextExR(MINECRAFT_BOLD, txt, {x, y}, SCALE(fontSize), 1, leftColor);
    }

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%i", (int)rightVal);
    Util::DrawRightAlignedText(MINECRAFT_BOLD, SCALE(fontSize), TEXT_BUFFER,
                               x + bounds.width, y, rightColor);
  }

  inline void DrawFormatCell(float x, float y, const char* txt, const char* format,
                             float val, const Color& numColor = Colors::darkBackground) {
    Update(x, y);

    snprintf(TEXT_BUFFER,TEXT_BUFFER_SIZE, format, val);

    Vector2 txtSize = MeasureTextEx(font, txt, SCALE(fontSize), 1);
    float numX = x + txtSize.x;

    if (isHovered) {
      ToolTip::DrawToolTipAtMouse(toolTip, font, fontSize);
      DrawTextExR(font, txt, {x, y}, SCALE(fontSize), 1, Colors::mediumLightGreyDarker);
      DrawTextExR(font, TEXT_BUFFER, {numX, y}, SCALE(fontSize), 1, numColor);
    } else {
      DrawTextExR(font, txt, {x, y}, SCALE(fontSize), 1, Colors::darkBackground);
      DrawTextExR(font, TEXT_BUFFER, {numX, y}, SCALE(fontSize), 1, numColor);
    }
  }

  static std::array<TextCell, 18> CreateCharacterCells(float width, float height,
                                                       const Font& font,
                                                       float fontSize) noexcept {
    auto toolTips = Util::LoadTextLines(ASSET_PATH + "Text/Codex/StatsToolTip.txt");
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

        TextCell(width + 25, height, toolTips[9], font, fontSize),
        TextCell(width + 25, height, toolTips[10], font, fontSize),
        TextCell(width + 25, height, toolTips[11], font, fontSize),
        TextCell(width + 25, height, toolTips[12], font, fontSize),
        TextCell(width + 25, height, toolTips[13], font, fontSize),
        TextCell(width + 25, height, toolTips[14], font, fontSize),
        TextCell(width + 25, height, toolTips[15], font, fontSize),
        TextCell(width + 25, height, toolTips[16], font, fontSize),
        TextCell(width + 25, height, toolTips[17], font, fontSize),
    };
    return cells;
  }
 private:
  inline void Update(float x, float y) noexcept {
    bounds.x = x;
    bounds.y = y;
    bounds.width = SCALE(baseWidth);
    bounds.height = SCALE(baseHeight);

    isHovered = CheckCollisionPointRec(MOUSE_POS, bounds);
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_TEXTCELL_H_
