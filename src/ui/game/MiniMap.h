#ifndef MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
#define MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_

struct MiniMap {
  static constexpr int width = 200;
  static constexpr int height = 200;
  static constexpr int zoom = 5;
  static constexpr int draw_y = 25;
  static constexpr int tile_width = width / zoom;
  void draw() const noexcept {
    int draw_x = SCREEN_WIDTH - 25 - width;
    int tile_x = PLAYER_TILE_X - 15;
    int tile_y = PLAYER_TILE_Y - 15;
    for (uint_fast32_t i = 0; i < tile_width; i++) {
      for (uint_fast32_t j = 0; j < tile_width; j++) {
        if (map_bounds(tile_x + i, tile_y + j)) {
          if (COLLISIONS[CURRENT_BACK_GROUND[tile_x + i][tile_y + j]] == C_SOLID ||
              COLLISIONS[CURRENT_MIDDLE_GROUND[tile_x + i][tile_y + j]] == C_SOLID) {
            DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                             Colors::mediumLightGreyTransparent);
          } else {
            if (tile_x + i == PLAYER_TILE_X && tile_y + j == PLAYER_TILE_Y) {
              DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                               Colors::blue_npc);

            } else {
              DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                               Colors::map_green);
            }
          }
        } else {
          DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                           Colors::black);
        }
      }
    }
  }

  void update() {}

  inline bool map_bounds(int x, int y) const noexcept {
    return x > 0 && y > 0 && x < CURRENT_MAP_SIZE && y < CURRENT_MAP_SIZE;
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
