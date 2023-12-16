#ifndef MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_

struct Button {
  static bool Draw(const RectangleR& bounds, const std::string& buttonText,
                   const std::string& toolTip) noexcept {
    bool ret = false;
    if (CheckCollisionPointRec(MOUSE_POS, bounds)) {
      ToolTip::DrawToolTipAtMouse(toolTip, VARNISHED, 15);
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawPressedButton(bounds);
      } else {
        DrawHoveredButton(bounds);
        ret = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
      }
    } else {
      DrawNormalButton(bounds);
    }
    DrawButtonText(bounds, buttonText, Alignment::MIDDLE);
    return ret;
  }


 private:
  inline static void DrawNormalButton(const RectangleR& bound) noexcept {
    DrawRectangleRounded(bound, 0.2F, GAME_SETTINGS.roundSegments, Colors::mediumVeryLight);
    DrawRectangleRoundedLines(bound, 0.2F, GAME_SETTINGS.roundSegments, 1, Colors::darkBackground);
  }
  inline static void DrawHoveredButton(const RectangleR& bound) noexcept {
    DrawRectangleRounded(bound, 0.2F, GAME_SETTINGS.roundSegments, Colors::mediumLightGreyBitDarker);
    DrawRectangleRoundedLines(bound, 0.2F, GAME_SETTINGS.roundSegments, 1, Colors::darkBackground);
  }
  inline static void DrawPressedButton(const RectangleR& bound) noexcept {
    DrawRectangleRounded(bound, 0.2F, GAME_SETTINGS.roundSegments, Colors::mediumLightGreyDarker);
    DrawRectangleRoundedLines(bound, 0.2F, GAME_SETTINGS.roundSegments, 1, Colors::darkBackground);
  }
  inline static void DrawButtonText(const RectangleR& bounds, const std::string& txt,
                                    Alignment align) noexcept {
    float fontSize = 14;
    switch (align) {
      case Alignment::LEFT: {
        auto bound = MeasureTextEx(MINECRAFT_BOLD, txt.c_str(), fontSize, 0.5);
        DrawTextExR(MINECRAFT_BOLD, txt.c_str(),
                    {fontSize + bounds.x, bounds.y + bounds.height / 2 - bound.y / 2},
                    fontSize, 0.5F, GetTextColor());
      }

      break;
      case Alignment::RIGHT:
        Util::DrawRightAlignedText(VARNISHED, fontSize, txt.c_str(),
                                   bounds.x + bounds.width - fontSize,
                                   bounds.y + bounds.height / 3.4F, GetTextColor());

        break;
      case Alignment::MIDDLE:
        Util::DrawCenteredText(MINECRAFT_BOLD, fontSize, txt.c_str(),
                               bounds.x + bounds.width / 2.0F,
                               bounds.y + bounds.height / 3.4F, GetTextColor());
        break;
    }
  }
  inline static Color GetTextColor() noexcept { return Colors::darkBackground; }
};

#endif  //MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
