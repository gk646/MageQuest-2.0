#ifndef MAGEQUEST_SRC_UI_ELEMENTS_TEXTUREDBUTTON_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_TEXTUREDBUTTON_H_

#include "ToolTip.h"

struct TexturedButton {
  std::function<void()> onPressedFunc;
  const std::string txt;
  const std::string toolTip;
  RectangleR bounds;
  const Texture& normal;
  const Texture& hovered;
  const Texture& pressed;
  float base_width, base_height;
  float fontSize = 15;
  bool isHovered = false;
  uint8_t alpha = 255;
  TexturedButton(float width, float height, const std::string& txt, float fontSize,
                 const Texture& normal, const Texture& hovered, const Texture& pressed,
                 uint8_t alpha = 255, std::string toolTip = "",
                 const std::function<void()>& func = nullptr)
      : bounds(0, 0, width, height),
        base_width(width),
        base_height(height),
        txt(txt),
        fontSize(fontSize),
        normal(normal),
        hovered(hovered),
        pressed(pressed),
        alpha(alpha),
        toolTip(std::move(toolTip)),
        onPressedFunc(func) {}
  bool Draw(float x, float y, Alignment textAlign = Alignment::MIDDLE,
            Alignment buttonAlign = Alignment::MIDDLE) noexcept {
    Update(x, y, buttonAlign);
    if (isHovered) {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawTextureScaled(pressed, bounds, 0, false, 0, {255, 255, 255, 255});
      } else {
        DrawTextureScaled(hovered, bounds, 0, false, 0, {255, 255, 255, 255});
      }
      ToolTip::DrawToolTip(toolTip, ANT_PARTY, fontSize);
    } else {
      DrawTextureScaled(normal, bounds, 0, false, 0, {255, 255, 255, alpha});
    }
    DrawButtonText(textAlign);
    return PlayConfirmSound();
  }
  [[nodiscard]] inline bool IsHovered() const noexcept { return isHovered; }
  inline void UpdateGlobalWindowState() const noexcept {
    if (isHovered) WINDOW_FOCUSED = true;
  }

 private:
  inline void DrawButtonText(Alignment align) noexcept {
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
        Util::DrawCenteredText(VARNISHED, fontSize, txt.c_str(),
                               bounds.x + bounds.width / 2.0F,
                               bounds.y + bounds.height / 3.4F, GetTextColor());
        break;
    }
  }
  [[nodiscard]] inline const Color& GetTextColor() const noexcept {
    if (isHovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      return Colors::LightGrey;
    }
    return Colors::darkBackground;
  }
  inline void Update(float x, float y, Alignment buttonAlign) noexcept {
    bounds.x = x;
    bounds.y = y;
    //bounds.width = SCALE(base_width);
    //bounds.height = SCALE(base_height);
    if (buttonAlign == Alignment::MIDDLE) {
      bounds.x -= bounds.width / 2;
    }

    isHovered = CheckCollisionPointRec(MOUSE_POS, bounds);
  }
  inline void PlayerEnterSound() {
    if (!isHovered) {
      PlaySoundR(sound::menu_switch);
      isHovered = true;
    }
  }
  [[nodiscard]] inline bool PlayConfirmSound() const noexcept {
    if (isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      if (onPressedFunc) {
        onPressedFunc();
      }
      PlaySoundR(sound::menu_switch);
      return true;
    }
    return false;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_TEXTUREDBUTTON_H_
