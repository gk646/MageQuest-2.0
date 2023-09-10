#ifndef MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_
#define MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_

struct RegionMap final : public Window {
  int zoom = 5;
  bool dragged = false;
  PointI tile_offset{};
  Vector2 last_mouse_pos;
  RegionMap()
      : Window(100, 100, SCREEN_WIDTH * 0.9, SCREEN_HEIGHT * 0.9, 20, "Region Map",
               KEY_M) {}
  void draw() noexcept {
    OPEN_CLOSE()
    DRAG_WINDOW()
    drag_map();
    draw_region_window();
    draw_region_map();
  }

  inline void draw_region_window() const noexcept {
    RectangleR scaled_whole = SCALE_RECT(whole_window);
    RectangleR scaled_head = SCALE_RECT(header_bar);

    DrawRectangleRounded(scaled_whole, 0.03F, 30, Colors::lightGreyMiddleAlpha);

    DrawRectangleRounded(scaled_head, 0.5F, 30,
                         header_hover ? isDragging ? Colors::mediumLightGreyDarker
                                                   : Colors::mediumLightGreyBitDarker
                                      : Colors::mediumLightGrey);

    DrawRectangleRoundedLines(scaled_whole, 0.03F, 30, 3, Colors::darkBackground);
    DrawRectangleRoundedLines(scaled_head, 0.5F, 30, 2, Colors::darkBackground);

    DrawTextExR(ANT_PARTY, header_text,
                {scaled_whole.x + scaled_whole.width / 2 -
                     GetTextWidth(header_text, font_size * UI_SCALE) / 2,
                 scaled_whole.y + scaled_head.height / 4},
                font_size * UI_SCALE, 1, Colors::darkBackground);
  }
  inline void draw_region_map() noexcept {
    auto player_tile = *PLAYER_TILE;
    if (!dragged) {
      tile_offset = *PLAYER_TILE;
    }

    int draw_x = whole_window.x + whole_window.width / 2 - tile_offset.x * zoom;
    int draw_y = whole_window.y + whole_window.height / 2 - tile_offset.y * zoom;

    for (int_fast32_t i = 0; i < CURRENT_MAP_SIZE; i++) {
      for (int_fast32_t j = 0; j < CURRENT_MAP_SIZE; j++) {
        if (MiniMap::map_bounds(i, j)) {
          if (COLLISIONS[CURRENT_BACK_GROUND[i][j]] == C_SOLID ||
              COLLISIONS[CURRENT_MIDDLE_GROUND[i][j]] == C_SOLID) {
            if (window_bounds(draw_x + i * zoom, draw_y + j * zoom)) {

              DrawRectangleProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                                   Colors::darkBackground);
            }
          } else [[likely]] {
            if (i == player_tile.x && j == player_tile.y) {
              if (window_bounds(draw_x + i * zoom, draw_y + j * zoom)) {
                DrawRectangleProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                                     Colors::Blue);
              }
            } else {
              if (window_bounds(draw_x + i * zoom, draw_y + j * zoom)) {
                DrawRectangleProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                                     Colors::map_green);
              }
            }
          }
        } else {
          if (window_bounds(draw_x + i * zoom, draw_y + j * zoom)) {
            DrawRectangleProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                                 Colors::lightGreyMiddleAlpha);
          }
        }
      }
    }
  }
  inline bool window_bounds(int x, int y) noexcept {
    return x >= whole_window.x && x < whole_window.x + whole_window.width &&
           y >= whole_window.y && y < whole_window.y + whole_window.height;
  }
  inline void drag_map() noexcept {
    auto mouse_pos = GetMousePosition();
    if (!isDragging && IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse_pos, whole_window)) {
      int deltax = mouse_pos.x - last_mouse_pos.x;
      int deltay = mouse_pos.y - last_mouse_pos.y;
      tile_offset.x -= deltax;
      tile_offset.y -= deltay;
      dragged = true;
    } else if (IsKeyPressed(KEY_SPACE)) {
      dragged = false;
    }
    last_mouse_pos = mouse_pos;

    int wheel = GetMouseWheelMove();
    if (zoom + wheel > 0) {
      zoom += wheel;
    }
  }
  void update() {
    update_window();
    if (whole_window.width < SCREEN_WIDTH * 0.9) {
      whole_window.width = SCREEN_WIDTH * 0.9;
      header_bar.width = SCREEN_WIDTH * 0.9;
    }
    if (whole_window.height < SCREEN_HEIGHT * 0.9) {
      whole_window.height = SCREEN_HEIGHT * 0.9;
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_
