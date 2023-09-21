#ifndef MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_
#define MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_

struct RegionMap final : public Window {
  float zoom = 5;
  bool dragged = false;
  PointI tile_offset{};
  Vector2 last_mouse_pos{};
  RegionMap()
      : Window(100, 100, SCREEN_WIDTH * 0.9, SCREEN_HEIGHT * 0.9, 20, "Region Map",
               KEY_M) {}
  void draw() noexcept {
    WINDOW_LOGIC();
    drag_map();
    draw_region_window();
    draw_region_map();
  }
  inline void draw_region_window() const noexcept {
    RectangleR scaled_whole = SCALE_RECT(whole_window);
    RectangleR scaled_head = SCALE_RECT(header_bar);

    DrawRectangleRounded(scaled_whole, 0.03F, ROUND_SEGMENTS,
                         Colors::lightGreyMiddleAlpha);

    DrawRectangleRounded(scaled_head, 0.5F, ROUND_SEGMENTS,
                         header_hover ? isDragging ? Colors::mediumLightGreyDarker
                                                   : Colors::mediumLightGreyBitDarker
                                      : Colors::mediumLightGrey);

    DrawRectangleRoundedLines(scaled_whole, 0.03F, ROUND_SEGMENTS, 3,
                              Colors::darkBackground);
    DrawRectangleRoundedLines(scaled_head, 0.5F, ROUND_SEGMENTS, 2,
                              Colors::darkBackground);

    DrawTextExR(ANT_PARTY, header_text,
                {scaled_whole.x + scaled_whole.width / 2 -
                     GetTextWidth(header_text, font_size * UI_SCALE) / 2,
                 scaled_whole.y + scaled_head.height / 4},
                font_size * UI_SCALE, 1, Colors::darkBackground);
  }
  inline void draw_region_map() noexcept {
    auto player_tile = *PLAYER_TILE;

    if (!dragged) {
      tile_offset = player_tile;
    }
    int curr_size = CURRENT_MAP_SIZE;
    float base_x = whole_window.x + whole_window.width / 2 - tile_offset.x * zoom;
    float base_y = whole_window.y + whole_window.height / 2 - tile_offset.y * zoom;
    float draw_x, draw_y;
    for (int_fast32_t i = 0; i < curr_size; i++) {
      draw_x = base_x + i * zoom;
      for (int_fast32_t j = 0; j < curr_size; j++) {
        draw_y = base_y + j * zoom;

        if (window_bounds(draw_x, draw_y)) [[likely]] {
          if (COLLISIONS[CURRENT_BACK_GROUND[i][j]] == C_SOLID ||
              COLLISIONS[CURRENT_MIDDLE_GROUND[i][j]] == C_SOLID) {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::darkBackground);
          } else if (i == player_tile.x && j == player_tile.y) {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::Blue);
          } else {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::map_green);
          }
        }
      }
    }
    if(FAST_UI){
      return;
    }
    std::shared_lock<std::shared_mutex> lock(rwLock);
    for (const auto monster : MONSTERS) {
      if (monster->tile_pos.dist(player_tile) < 20) {
        DrawSquareProFast(base_x + monster->tile_pos.x * zoom,
                          base_y + monster->tile_pos.y * zoom, zoom, Colors::Red);
      }
    }
    for (const auto projectile : PROJECTILES) {
      if (projectile->from_player) {
        DrawSquareProFast(base_x + projectile->tile_pos.x * zoom,
                          base_y + projectile->tile_pos.y * zoom, 3, Colors::Blue);
      } else {
        DrawSquareProFast(base_x + projectile->tile_pos.x * zoom,
                          base_y + projectile->tile_pos.y * zoom, 3, Colors::Red);
      }
    }
  }
  [[nodiscard]] inline bool window_bounds(float x, float y) const noexcept {
    return x >= whole_window.x && x < whole_window.x + whole_window.width &&
           y >= whole_window.y && y < whole_window.y + whole_window.height;
  }
  inline void drag_map() noexcept {
    auto mouse_pos = MOUSE_POS;
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

    float wheel = GetMouseWheelMove();
    if (zoom + wheel > 0 && zoom + wheel < 14) {
      zoom += wheel;
    }
  }
  void update() {
    WINDOW_UPDATE()
    if (whole_window.width < SCREEN_WIDTH * 0.9) {
      whole_window.width = SCREEN_WIDTH * 0.9;
      header_bar.width = SCREEN_WIDTH * 0.9;
      whole_window.height = SCREEN_HEIGHT * 0.9;
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_
