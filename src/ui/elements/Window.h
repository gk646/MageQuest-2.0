#ifndef MAGEQUEST_SRC_UI_WINDOW_H_
#define MAGEQUEST_SRC_UI_WINDOW_H_

struct Window {
  RectangleR whole_window;
  RectangleR header_bar;
  Vector2 lastMousePos = {0};
  Vector2 base_pos;
  char* header_text;
  int open_key;
  float font_size = 17;
  bool isDragging = false;
  bool open = false;
  bool header_hover = false;
  Window(int start_x, int start_y, int width, int height, int header_height,
         char* header_text, int open_key)
      : whole_window(start_x, start_y, width, height),
        header_bar(start_x, start_y + 2, width, header_height),
        header_text(header_text),
        open_key(open_key),
        base_pos(start_x, start_y) {}

#define WINDOW_LOGIC()                                              \
  if (IsKeyPressed(open_key)) {                                     \
    open = !open;                                                   \
    isDragging = false;                                             \
  }                                                                 \
                                                                    \
  if (!open) {                                                      \
    return;                                                         \
  }                                                                 \
                                                                    \
  if (isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {         \
    auto mouse_pos = MOUSE_POS;                                     \
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

#define DRAG_WINDOW()                                               \
  if (isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {         \
    auto mouse_pos = MOUSE_POS;                                     \
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

  void DrawWindow() const noexcept {
    RectangleR scaled_whole = SCALE_RECT(whole_window);
    RectangleR scaled_head = SCALE_RECT(header_bar);

    float ROUNDNESS = 0.1F;
    if (whole_window.width > 450) {
      ROUNDNESS = 0.05F;
    }

    DrawRectangleRounded(scaled_whole, ROUNDNESS, ROUND_SEGMENTS, Colors::LightGrey);

    DrawRectangleRounded(scaled_head, 0.5F, ROUND_SEGMENTS,
                         header_hover ? isDragging ? Colors::mediumLightGreyDarker
                                                   : Colors::mediumLightGreyBitDarker
                                      : Colors::mediumLightGrey);

    DrawRectangleRoundedLines(scaled_head, 1.5F, ROUND_SEGMENTS, 2,
                              Colors::darkBackground);
    DrawRectangleRoundedLines(scaled_whole, ROUNDNESS, ROUND_SEGMENTS, 3,
                              Colors::darkBackground);
    DrawCenteredText(ANT_PARTY, SCALE(font_size), header_text,
                     scaled_whole.x + scaled_whole.width / 2,
                     scaled_whole.y + scaled_head.height / 4, Colors::darkBackground);
  }
#define WINDOW_UPDATE()                                                             \
  if (!open) {                                                                      \
    return;                                                                         \
  }                                                                                 \
  header_hover = false;                                                             \
  if (CheckCollisionPointRec(MOUSE_POS, SCALE_RECT(header_bar)) && !DRAGGED_ITEM) { \
    header_hover = true;                                                            \
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {                                     \
      if (!isDragging) {                                                            \
        isDragging = true;                                                          \
        lastMousePos = MOUSE_POS;                                                   \
      }                                                                             \
    }                                                                               \
  }                                                                                 \
                                                                                    \
  if (!WINDOW_FOCUSED) {                                                            \
    WINDOW_FOCUSED =                                                                \
        isDragging || CheckCollisionPointRec(MOUSE_POS, SCALE_RECT(whole_window));  \
  }
  inline void ToggleWindow() noexcept {
    if (!open) {
      open = true;
    } else {
      open = false;
    }
  }
  inline void ResetPosition() noexcept {
    whole_window.x = base_pos.x;
    whole_window.y = base_pos.y;

    header_bar.x = base_pos.x;
    header_bar.y = base_pos.y + 2;
  }
};
#endif  //MAGEQUEST_SRC_UI_WINDOW_H_
