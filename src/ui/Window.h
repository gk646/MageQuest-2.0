#ifndef MAGEQUEST_SRC_UI_WINDOW_H_
#define MAGEQUEST_SRC_UI_WINDOW_H_

struct Window {
  Rectangle whole_window;
  Rectangle header_bar;
  Vector2 lastMousePos = {0};
  bool isDragging = false;
  const char* header_text;
  int open_key;
  int font_size = 17;
  bool open = false;
  bool header_hover = false;
  Vector2 base_pos;
  Window(int startx, int starty,int width, int height, int header_height, const char* header_text, int open_key)
      : whole_window(startx, starty, width, height),
        header_bar(startx, starty+2, width, header_height),
        header_text(header_text),
        open_key(open_key),
        base_pos(startx,starty) {}

#define OPEN_CLOSE()            \
  if (IsKeyPressed(open_key)) { \
    open = !open;               \
    isDragging = false;         \
  }                             \
                                \
  if (!open) {                  \
    return;                     \
  }
#define DRAG_WINDOW()                                               \
  if (isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {         \
    auto mouse_pos = GetMousePosition();                            \
    auto delta_x = (mouse_pos.x - lastMousePos.x) * (1 / UI_SCALE); \
    auto delta_y = (mouse_pos.y - lastMousePos.y) * (1 / UI_SCALE); \
    whole_window.x += delta_x;                                      \
    whole_window.y += delta_y;                                      \
    header_bar.x += delta_x;                                        \
    header_bar.y += delta_y;                                        \
    lastMousePos = mouse_pos;                                       \
  } else {                                                          \
    isDragging = false;                                             \
  }

  void draw_window() const noexcept {
    Rectangle scaled_whole = SCALE_RECT(whole_window);
    Rectangle scaled_head = SCALE_RECT(header_bar);

    DrawRectangleRounded(scaled_whole, 0.1F, 25, Colors::LightGrey);

    DrawRectangleRounded(scaled_head, 0.6F, 20,
                         header_hover ? isDragging ? Colors::mediumLightGreyDarker
                                                   : Colors::mediumLightGreyBitDarker
                                      : Colors::mediumLightGrey);

    DrawRectangleRoundedLines(scaled_whole, 0.1F, 25, 3, Colors::darkBackground);
    DrawRectangleRoundedLines(scaled_head, 1.5F, 15, 2, Colors::darkBackground);

    DrawTextEx(ANT_PARTY, header_text,
               {scaled_whole.x + scaled_whole.width / 2 -
                    GetTextWidth(header_text, font_size * UI_SCALE) / 2,
                scaled_whole.y + scaled_head.height / 4},
               font_size * UI_SCALE, 1, Colors::darkBackground);
  }
  void update_window() noexcept {
    if (!open) {
      return;
    }
    header_hover = false;
    if (CheckCollisionPointRec(GetMousePosition(), SCALE_RECT(header_bar))&&!DRAGGED_ITEM) {
      header_hover = true;
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (!isDragging) {
          isDragging = true;
          lastMousePos = GetMousePosition();
        }
      }
    }

    if (!WINDOW_FOCUSED) {
      WINDOW_FOCUSED = isDragging || CheckCollisionPointRec(GetMousePosition(),
                                                            SCALE_RECT(whole_window));
    }
  }
  inline void reset_pos() noexcept {
    whole_window.x = base_pos.x;
    whole_window.y = base_pos.y;

    header_bar.x = base_pos.x;
    header_bar.y = base_pos.y+2;
  }
};
#endif  //MAGEQUEST_SRC_UI_WINDOW_H_
