#ifndef MAGEQUEST_SRC_UI_ELEMENTS_PANEL_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_PANEL_H_
struct Panel {
  float baseWidth;
  float baseHeight;
  float baseHeaderHeight;
  RectangleR header;
  RectangleR body;
  bool expanded = false;
  char* headerText;
  const Font& font;
  bool isHovered = false;
  Panel(float width, float height, float headerHeight, const Font& font, char* headerText)
      : header(0, 0, width, headerHeight),
        body(0, 0, width, height),
        baseHeight(height),
        baseHeaderHeight(headerHeight),
        baseWidth(width),
        headerText(headerText),
        font(font) {}

  void Draw(float x, float y) {
    UpdateInternal(x, y);
    if (expanded) {

    } else {
      if (isHovered) {

        DrawRectangleRounded(header, 0.2F, ROUND_SEGMENTS, Colors::LightGreyAlpha);
      }

      DrawTextExR(font, headerText, {x + 5, y + 4}, SCALE(15), 1, Colors::darkBackground);
    }
  }
  virtual void DrawContent() noexcept = 0;

  void Update() {
   isHovered = CheckCollisionPointRec(MOUSE_POS, header);
    if(isHovered) WINDOW_FOCUSED = true;
  }

 private:
  inline void UpdateInternal(float x, float y) {
    header.x = x;
    header.y = y;
    header.width = SCALE(baseWidth);
    header.height = SCALE(baseHeaderHeight);

    body.x = x;
    body.y = y;
    body.width = header.width;
    body.height = SCALE(baseHeight);
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_PANEL_H_
