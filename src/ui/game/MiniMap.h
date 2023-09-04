#ifndef MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
#define MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_

class MiniMap {
  static constexpr int width = 200;
  static constexpr int height = 200;
  static constexpr int zoom = 5;
  static constexpr int draw_y = 35;
  static constexpr int tile_width = width / zoom;
  std::stringstream ss{};
  std::string time_str{};
  int update_timer = 500;
  inline bool map_bounds(int x, int y) const noexcept {
    return x > 0 && y > 0 && x < CURRENT_MAP_SIZE && y < CURRENT_MAP_SIZE;
  }

 public:
  void draw() const noexcept {
    int draw_x = SCREEN_WIDTH - 25 - width;
    int tile_x = PLAYER_TILE_X - 15;
    int tile_y = PLAYER_TILE_Y - 15;

    DrawTextEx(ANT_PARTY, zoneMap[CURRENT_ZONE].c_str(), {(float)draw_x - 20, 9}, 17, 1,
               Colors::white);
    DrawTextEx(ANT_PARTY, time_str.c_str(), {SCREEN_WIDTH - 55, 9}, 17, 1, Colors::white);

    DrawRectangleLinesEx({(float)draw_x - 3, draw_y - 3, width + 6, height + 6}, 3,
                         Colors::LightGrey);
    for (uint_fast32_t i = 0; i < tile_width; i++) {
      for (uint_fast32_t j = 0; j < tile_width; j++) {
        if (map_bounds(tile_x + i, tile_y + j)) {
          if (COLLISIONS[CURRENT_BACK_GROUND[tile_x + i][tile_y + j]] == C_SOLID ||
              COLLISIONS[CURRENT_MIDDLE_GROUND[tile_x + i][tile_y + j]] == C_SOLID) {
            DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                             Colors::darkBackground);
          } else {
            if (tile_x + i == PLAYER_TILE_X && tile_y + j == PLAYER_TILE_Y) {
              DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                               Colors::Blue);

            } else {
              DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                               Colors::map_green);
            }
          }
        } else {
          DrawRectanglePro(draw_x + i * zoom, draw_y + j * zoom, zoom, zoom, {0, 0}, 0,
                           Colors::lightGreyMiddleAlpha);
        }
      }
    }
  }
  void update() noexcept {
    if (update_timer >= 500) {
      update_timer = 0;
      std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm* local_time = std::localtime(&now_time);

      ss.str("");
      ss.clear();

      ss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":" << std::setw(2)
         << std::setfill('0') << local_time->tm_min;

      time_str = ss.str();
    }
    update_timer++;
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
