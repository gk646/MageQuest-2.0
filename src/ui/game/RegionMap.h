#ifndef MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_
#define MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_

struct RegionMap final : public Window {
  Vector2 last_mouse_pos{};
  PointT<int16_t> tile_offset{};
  float zoom = 5;
  bool dragged = false;
  RegionMap()
      : Window(100, 100, SCREEN_WIDTH * 0.9F, SCREEN_HEIGHT * 0.9F, 20, "Map", KEY_M, sound::EMPTY_SOUND, sound::EMPTY_SOUND) {}
  void Draw() noexcept {
    WINDOW_LOGIC();
    DragMap();
    DrawCustomWindow();
    DrawMap();
  }
  void Update() {
    WINDOW_UPDATE()
    if (wholeWindow.width < SCREEN_WIDTH * 0.9) {
      wholeWindow.width = SCREEN_WIDTH * 0.9;
      header_bar.width = SCREEN_WIDTH * 0.9;
      wholeWindow.height = SCREEN_HEIGHT * 0.9;
    }
  }

 private:
  void DrawMap() noexcept {
    auto player_tile = *PLAYER_TILE;

    if (!dragged) {
      tile_offset = player_tile;
    }

    int curr_size = CURRENT_MAP_SIZE;
    float base_x = wholeWindow.x + wholeWindow.width / 2 - tile_offset.x * zoom;
    float base_y = wholeWindow.y + wholeWindow.height / 2 - tile_offset.y * zoom;
    float draw_x, draw_y;
    for (int_fast32_t i = 0; i < curr_size; i++) {
      draw_x = base_x + i * zoom;
      for (int_fast32_t j = 0; j < curr_size; j++) {
        draw_y = base_y + j * zoom;
        if (IsInsideWindowBounds(draw_x, draw_y)) [[likely]] {
          if (IsTileCovered(i, j)) {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::black);
            continue;
          }
          if (CheckTileCollision(i, j)) {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::darkBackground);
          } else if (i == player_tile.x && j == player_tile.y) {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::Blue);
          } else {
            DrawSquareProFast(draw_x, draw_y, zoom, Colors::map_green);
          }
        }
      }
    }

    if (FAST_UI) {
      return;
    }

    for (const auto monster : MONSTERS) {
      if (monster->tilePos.dist(player_tile) < 20) {
        DrawSquareProFast(base_x + monster->tilePos.x * zoom,
                          base_y + monster->tilePos.y * zoom, zoom, Colors::Red);
      }
    }
    for (const auto projectile : PROJECTILES) {
      if (projectile->isFriendlyToPlayer) {
        DrawSquareProFast(base_x + projectile->tilePos.x * zoom,
                          base_y + projectile->tilePos.y * zoom, 0.6F * zoom,
                          Colors::Blue);
      } else {
        DrawSquareProFast(base_x + projectile->tilePos.x * zoom,
                          base_y + projectile->tilePos.y * zoom, 0.6F * zoom,
                          Colors::Red);
      }
    }

    PointT<int16_t>  questWaypoint;
    if (PLAYER_QUESTS.activeQuest &&
        (questWaypoint = PLAYER_QUESTS.activeQuest->GetActiveWaypoint()) != 0) {
      DrawSquareProFast(base_x + questWaypoint.x * zoom, base_y + questWaypoint.y * zoom,
                        zoom, Colors::questMarkerYellow);
    }
  }
  void DragMap() noexcept {
    auto mouse_pos = MOUSE_POS;
    if (!isDragged && IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse_pos, wholeWindow)) {
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
  void DrawCustomWindow() const noexcept {
    RectangleR scaled_whole = SCALE_RECT(wholeWindow);
    RectangleR scaled_head = SCALE_RECT(header_bar);

    DrawRectangleRounded(scaled_whole, 0.03F, ROUND_SEGMENTS,
                         Colors::lightGreyMiddleAlpha);

    DrawRectangleRounded(scaled_head, 0.5F, ROUND_SEGMENTS,
                         isHeaderHovered ? isDragged ? Colors::mediumLightGreyDarker
                                                     : Colors::mediumLightGreyBitDarker
                                         : Colors::mediumLightGrey);

    DrawRectangleRoundedLines(scaled_whole, 0.03F, ROUND_SEGMENTS, 3,
                              Colors::darkBackground);
    DrawRectangleRoundedLines(scaled_head, 0.5F, ROUND_SEGMENTS, 2,
                              Colors::darkBackground);

    DrawTextExR(ANT_PARTY, header_text,
                {scaled_whole.x + scaled_whole.width / 2.0F -
                     GetTextWidth(header_text, fontSize * UI_SCALE) / 2.0F,
                 scaled_whole.y + scaled_head.height / 4.0F},
                fontSize * UI_SCALE, 1, Colors::darkBackground);
  }
};
#endif  //MAGEQUEST_SRC_UI_GAME_REGIONMAP_H_
