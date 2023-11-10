#ifndef MAGEQUEST_SRC_UI_ELEMENTS_TOOLTIP_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_TOOLTIP_H_
namespace ToolTip {
inline void DrawToolTipAtMouse(const std::string& txt, const Font& font,
                        float fontSize) noexcept {
  if (txt.empty()) return;
  Vector2 txtBounds = MeasureTextEx(font, txt.c_str(), SCALE(fontSize), 1);

  DrawTextureScaled(
      textures::ui::toolTip,
      {MOUSE_POS.x, MOUSE_POS.y - txtBounds.y * 1.5F, txtBounds.x + 15, txtBounds.y + 5},
      0, false, 0, WHITE);
  DrawTextExR(font, txt.c_str(), {MOUSE_POS.x + 12, MOUSE_POS.y - txtBounds.y * 1.3F},
              SCALE(fontSize), 1, Colors::mediumVeryLight);
}
}  // namespace ToolTip
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_TOOLTIP_H_
