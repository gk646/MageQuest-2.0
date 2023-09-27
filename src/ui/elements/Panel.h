#ifndef MAGEQUEST_SRC_UI_ELEMENTS_PANEL_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_PANEL_H_
struct Panel {
  float baseWidth;
  float baseHeight;
  float baseHeaderHeight;
  RectangleR button;
  RectangleR body;
  bool expanded = false;
  char* headerText;
  const Font& font;
  bool isHovered = false;
  Panel(float width, float height, float headerHeight, const Font& font, char* headerText)
      : button(0, 0, 16, 16),
        body(0, 0, width, height),
        baseHeight(height),
        baseHeaderHeight(headerHeight),
        baseWidth(width),
        headerText(headerText),
        font(font) {}

  void Draw(float x, float y) {
    UpdateInternal(x, y);
    if (expanded) {
      DrawTextureScaled(textures::ui::buttonCollapse, button, 0, false, 0, WHITE);
      if (isHovered) {
        DrawTextureScaled(textures::ui::buttonCollapseHovered, button, 0, false, 0,
                          WHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) expanded = false;
      }
      DrawContent();
    } else {
      DrawTextureScaled(textures::ui::buttonExpand, button, 0, false, 0, WHITE);
      if (isHovered) {
        DrawTextureScaled(textures::ui::buttonExpandHovered, button, 0, false, 0, WHITE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) expanded = true;
      }
      DrawOutlineText(font, SCALE(16), headerText, x + 5, y + 4, 1, Colors::LightGrey,
                      Colors::black);
    }
  }
  virtual void DrawContent() noexcept = 0;

  void Update() {
    isHovered = CheckCollisionPointRec(MOUSE_POS, button);
    if (isHovered) WINDOW_FOCUSED = true;
  }

 private:
  inline void UpdateInternal(float x, float y) {
    body.x = x;
    body.y = y;
    body.width = SCALE(baseWidth);
    body.height = SCALE(baseHeight);

    button.x = body.x + body.width - SCALE(20);
    button.y = body.y + SCALE(2);
    button.width = SCALE(baseHeaderHeight);
    button.height = SCALE(baseHeaderHeight);
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_PANEL_H_
