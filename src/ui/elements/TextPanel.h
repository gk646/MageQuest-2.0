#ifndef MAGEQUEST_SRC_UI_ELEMENTS_TEXTPANEL_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_TEXTPANEL_H_
struct TextPanel {
  RectangleR bounds{};
  std::string text;
  float fontSize;
  bool isOpen = false;
  //Bounds are calculated based on the text
  TextPanel(float width, const std::string& text, float fontSize)
      : text(text), fontSize(fontSize) {
    int lineBreaks = 0;
    auto textWrap = Util::WrapText(text, width, MINECRAFT_REGULAR, fontSize, &lineBreaks);
    bounds.width = width;
    bounds.height = 25.0F + (float)lineBreaks * fontSize;
  }

  inline void Draw(float x, float y) noexcept {
    if (!isOpen) return;
    UpdateImpl(x, y);
    DrawRectangleRounded(bounds, 0.2F, ROUND_SEGMENTS, Colors::LightGrey);
    DrawRectangleRoundedLines(bounds, 0.2F, ROUND_SEGMENTS, 2, Colors::darkBackground);

    DrawTextExR(MINECRAFT_REGULAR, text.c_str(), {bounds.x + 5, bounds.y + 5}, fontSize,
                0.5F, Colors::darkBackground);
  }
  inline void ReCalc() noexcept {
    int lineBreaks = 0;
    for (auto c : text) {
      if (c == '\n') {
        lineBreaks++;
      }
    }
    bounds.height = 25.0F + lineBreaks * fontSize;
  }

 private:
  inline void UpdateImpl(float x, float y) noexcept {
    bounds.x = x;
    bounds.y = y;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_TEXTPANEL_H_
