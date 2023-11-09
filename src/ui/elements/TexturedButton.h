#ifndef MAGEQUEST_SRC_UI_ELEMENTS_TEXTUREDBUTTON_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_TEXTUREDBUTTON_H_

#include "ToolTip.h"

struct TexturedButton {
  //Callback function triggered when the button is pressed
  std::function<void()> onPressedFunc;

  //Display text on the button
  const std::string txt;

  //Tooltip text that appears on hover
  const std::string toolTip;

  //The button's bounding box for drawing and collision detection
  RectangleR bounds;

  //Textures for different button states
  const Texture& normal;
  const Texture& hovered;
  const Texture& pressed;

  //Original dimensions of the button for scaling purposes
  float base_width, base_height;

  //Font size of the button's text
  float fontSize = 15;

  //Indicates whether the mouse is currently over the button
  bool isHovered = false;

  //If the button is
  bool isCovered = false;

  //Opacity of the button
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
  //Draws the button and returns true if it was clicked // execute  "onPressedFunc" automatically
  bool Draw(const float x, const float y, const Alignment textAlign = Alignment::MIDDLE,
            Alignment buttonAlign = Alignment::MIDDLE) noexcept {
    UpdateImpl(x, y, buttonAlign);
    //Render the button based on its state
    if (!isCovered && isHovered) {
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
    return CheckForClick();
  }
  //Updates the global window state when the button is hovered
  inline void UpdateGlobalWindowState() noexcept {
    isCovered = WINDOW_FOCUSED;
    if (isHovered) {
      WINDOW_FOCUSED = true;
    }
  }

 private:
  [[nodiscard]] inline bool CheckForClick() const noexcept {
    if (!isCovered && isHovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (onPressedFunc) {
        onPressedFunc();
      }
      PlaySoundR(sound::menu_switch);
      return true;
    }
    return false;
  }
  //Draws the button's text based on the specified alignment
  inline void DrawButtonText(const Alignment align) noexcept {
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
  inline void UpdateImpl(const float x, const float y, const Alignment buttonAlign) noexcept {
    bounds.x = x;
    bounds.y = y;
    //bounds.width = SCALE(base_width);
    //bounds.height = SCALE(base_height);
    if (buttonAlign == Alignment::MIDDLE) {
      bounds.x -= bounds.width / 2;
    }

    isHovered = CheckCollisionPointRec(MOUSE_POS, bounds);
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_TEXTUREDBUTTON_H_
