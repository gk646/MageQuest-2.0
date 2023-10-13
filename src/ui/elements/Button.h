#ifndef MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_

struct Button {
  static bool Draw(RectangleR bounds, const std::string& buttonText,
                   const std::string& toolTip) noexcept {
    if (CheckCollisionPointRec(MOUSE_POS, bounds)) {
      ToolTip::DrawToolTip(toolTip, ANT_PARTY, 15);
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height,
                      Colors::lightGreyMiddleAlpha);
      } else {
        DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height,
                      Colors::lightGreyMiddleAlpha);
      }
    } else {
      DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height,
                    Colors::lightGreyMiddleAlpha);
    }
    DrawButtonText(bounds, buttonText, TextAlign::MIDDLE);
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  }

 private:
  inline static void DrawButtonText(const RectangleR& bounds, const std::string& txt,
                                    TextAlign align) noexcept {
    float fontSize = 15;
    switch (align) {
      case TextAlign::LEFT: {
        auto bound = MeasureTextEx(MINECRAFT_BOLD, txt.c_str(), fontSize, 0.5);
        DrawTextExR(MINECRAFT_BOLD, txt.c_str(),
                    {fontSize + bounds.x, bounds.y + bounds.height / 2 - bound.y / 2},
                    fontSize, 0.5F, GetTextColor());
      }

      break;
      case TextAlign::RIGHT:
        DrawRightAlignedText(VARNISHED, fontSize, txt.c_str(),
                             bounds.x + bounds.width - fontSize,
                             bounds.y + bounds.height / 3.4F, GetTextColor());

        break;
      case TextAlign::MIDDLE:
        DrawCenteredText(VARNISHED, fontSize, txt.c_str(), bounds.x + bounds.width / 2.0F,
                         bounds.y + bounds.height / 3.4F, GetTextColor());
        break;
    }
  }
  inline static Color GetTextColor() noexcept { return Colors::darkBackground; }
};

#endif  //MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
