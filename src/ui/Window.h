#ifndef MAGEQUEST_SRC_UI_WINDOW_H_
#define MAGEQUEST_SRC_UI_WINDOW_H_

struct Window {
  Rectangle whole_window;
#define SCALE(val) ((val) * UI_SCALE)
#define SCALE_RECT(rect) { SCALE((rect).x), SCALE((rect).y), SCALE((rect).width), SCALE((rect).height) }
#define SCALE_TEXT_SIZE(size) (SCALE(size))
  Rectangle header_bar;
  bool isDragging = false;
  Vector2 lastMousePos;
  int open_key;
  bool open = false;
  const char* header_text;
  int font_size = 17;
  Window(int width, int height, int header_height, const char* header_text, int open_key)
      : whole_window(100, 100, width, height),
        header_bar(100, 100, width, header_height),
        header_text(header_text),
        open_key(open_key) {}
  void draw_window() {
    if (IsKeyPressed(open_key)) {
      open = !open;
      isDragging = false;
    }
    if (!open) {
      return;
    }

    Rectangle scaled_whole = SCALE_RECT(whole_window);
    Rectangle scaled_head = SCALE_RECT(header_bar);
    DrawRectangleRounded(scaled_whole, 0.1F, 25, Colors::LightGrey);
    DrawRectangleRoundedLines(scaled_whole, 0.1F, 25, 4, Colors::mediumLightGrey);
    DrawRectangleRoundedLines(scaled_head, 1.1F, 30, 4, Colors::mediumLightGrey);
    DrawText(header_text, scaled_whole.x + scaled_whole.width / 2 - GetTextWidth(header_text,font_size*UI_SCALE) / 2,
             scaled_whole.y + (header_bar.height *UI_SCALE) / 4, font_size*UI_SCALE, Colors::mediumLightGrey);
  }
  void update_window() {
    if (!open) {
      return;
    }
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, SCALE_RECT(header_bar))) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (!isDragging) {
          isDragging = true;
          lastMousePos = mousePos;
        }
      }
    }

    if (isDragging) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        whole_window.x += (mousePos.x - lastMousePos.x)*(1/UI_SCALE);
        whole_window.y += (mousePos.y - lastMousePos.y)*(1/UI_SCALE);
        header_bar.x = whole_window.x;
        header_bar.y = whole_window.y;

        lastMousePos = mousePos;
      } else {
        isDragging = false;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_WINDOW_H_
