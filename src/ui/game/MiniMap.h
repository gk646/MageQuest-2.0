#ifndef MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
#define MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_

struct MiniMap {
  static constexpr int width = 200;
  static constexpr int height = 200;
  static constexpr int zoom = 5;
  static constexpr float draw_y = 35;
  static constexpr int tile_width = width / zoom;
  std::stringstream ss{};
  std::string time_str{};
  int update_timer = 500;
  bool* region_map_open;

  explicit MiniMap(bool* region_map_status) : region_map_open(region_map_status) {}
  static inline bool map_bounds(int x, int y) noexcept {
    return x >= 0 && y >= 0 && x < CURRENT_MAP_SIZE && y < CURRENT_MAP_SIZE;
  }
  void draw() const noexcept {
    auto player_tile = *PLAYER_TILE;
    float draw_x = SCREEN_WIDTH - 25 - width;
    int tile_x = player_tile.x - 15;
    int tile_y = player_tile.y - 15;

    DrawTextExR(ANT_PARTY, zoneMap[CURRENT_ZONE].c_str(), {(float)draw_x - 20, 9}, 17, 1,
                Colors::white);
    DrawTextExR(ANT_PARTY, time_str.c_str(), {SCREEN_WIDTH - 55, 9}, 17, 1,
                Colors::white);

    DrawRectangleLinesEx({(float)draw_x - 3, draw_y - 3, width + 6, height + 6}, 3,
                         Colors::LightGrey);
    if (*region_map_open) {
      return;
    }
    for (int_fast32_t i = 0; i < tile_width; i++) {
      for (int_fast32_t j = 0; j < tile_width; j++) {
        if (map_bounds(tile_x + i, tile_y + j)) {
          if (COLLISIONS[CURRENT_BACK_GROUND[tile_x + i][tile_y + j]] == C_SOLID ||
              COLLISIONS[CURRENT_MIDDLE_GROUND[tile_x + i][tile_y + j]] == C_SOLID)
              [[unlikely]] {
            DrawSquareProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                              Colors::darkBackground);
          } else {
            if (tile_x + i == player_tile.x && tile_y + j == player_tile.y) {
              DrawSquareProFast(draw_x + i * zoom, draw_y + j * zoom, zoom, Colors::Blue);

            } else {
              DrawSquareProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                                Colors::map_green);
            }
          }
        } else {
          DrawSquareProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                            Colors::lightGreyMiddleAlpha);
        }
      }
    }
    if(FAST_UI){
      return;
    }
    std::shared_lock<std::shared_mutex> lock(rwLock);
    for (const auto projectile : PROJECTILES) {
      if(projectile->from_player){
        DrawSquareProFast(draw_x + (projectile->tile_pos.x - tile_x) * zoom,
                          draw_y + (projectile->tile_pos.y - tile_y) * zoom, 3,
                          Colors::Blue);
      }else{
        DrawSquareProFast(draw_x + (projectile->tile_pos.x - tile_x) * zoom,
                          draw_y + (projectile->tile_pos.y - tile_y) * zoom, 3,
                          Colors::Red);
      }
    }
    for (const auto monster : MONSTERS) {
      if (monster->tile_pos.dist(player_tile) < 20) {
        DrawSquareProFast(draw_x + (monster->tile_pos.x - tile_x) * zoom,
                          draw_y + (monster->tile_pos.y - tile_y) * zoom, zoom,
                          Colors::Red);
      }
    }
    for(const auto npc : NPCS){
      if (npc->tile_pos.dist(player_tile) < 20) {
        if(npc->id == NPC_ID::RANDOM || npc->id == NPC_ID::VILLAGER){
          DrawSquareProFast(draw_x + (npc->tile_pos.x - tile_x) * zoom,
                            draw_y + (npc->tile_pos.y - tile_y) * zoom, zoom,
                            Colors::green_npc);
        }else {
          DrawSquareProFast(draw_x + (npc->tile_pos.x - tile_x) * zoom,
                            draw_y + (npc->tile_pos.y - tile_y) * zoom, zoom,
                            Colors::blue_npc);
        }
      }
    }
  }
  void update() noexcept {
    if (update_timer >= 500) {
      update_timer = 0;
      std::time_t now_time =
          std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::tm* local_time = std::localtime(&now_time);

      ss.str("");
      ss.clear();

      ss << std::setw(2) << std::setfill('0') << local_time->tm_hour << ":"
         << std::setw(2) << std::setfill('0') << local_time->tm_min;

      time_str = ss.str();
    }
    update_timer++;
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
