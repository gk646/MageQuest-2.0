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
  static inline bool BoundCheckMap(int x, int y) noexcept {
    return x >= 0 && y >= 0 && x < CURRENT_MAP_SIZE && y < CURRENT_MAP_SIZE;
  }
  static inline bool BoundCheckObject(const Entity* e, int tile_x, int tile_y) noexcept {
    return !e->active || e->tile_pos.x < tile_x || e->tile_pos.y < tile_y ||
           e->tile_pos.x >= tile_x + tile_width || e->tile_pos.y >= tile_y + tile_width;
  }
  void Draw() const noexcept {
    auto player_tile = PLAYER.tile_pos;
    float draw_x = SCREEN_WIDTH - 25 - width;
    int tile_x = player_tile.x - tile_width / 2;
    int tile_y = player_tile.y - tile_width / 2;

    DrawTextExR(ANT_PARTY, zoneMap[CURRENT_ZONE].c_str(), {(float)draw_x - 20, 9}, 17, 1,
                Colors::white);
    DrawTextExR(ANT_PARTY, time_str.c_str(), {SCREEN_WIDTH - 55, 9}, 17, 1,
                Colors::white);

    DrawRectangleLinesEx({(float)draw_x - 3, draw_y - 3, width + 6, height + 6}, 3,
                         Colors::LightGrey);
    if (*region_map_open && FAST_UI) {
      return;
    }

    for (int_fast32_t i = 0; i < tile_width; i++) {
      for (int_fast32_t j = 0; j < tile_width; j++) {
        if (BoundCheckMap(tile_x + i, tile_y + j)) [[likely]] {
          if (CheckTileCollision(tile_x + i, tile_y + j)) [[unlikely]] {
            DrawSquareProFast(draw_x + i * zoom, draw_y + j * zoom, zoom,
                              Colors::darkBackground);
          } else {
            if (tile_x + i == player_tile.x && tile_y + j == player_tile.y) [[unlikely]] {
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

    if (FAST_UI) {
      return;
    }

    for (const auto projectile : PROJECTILES) {
      if (BoundCheckObject(projectile, tile_x, tile_y)) continue;

      if (projectile->from_player) {
        DrawSquareProFast(draw_x + (projectile->tile_pos.x - tile_x) * zoom,
                          draw_y + (projectile->tile_pos.y - tile_y) * zoom, 3,
                          Colors::Blue);
      } else {
        DrawSquareProFast(draw_x + (projectile->tile_pos.x - tile_x) * zoom,
                          draw_y + (projectile->tile_pos.y - tile_y) * zoom, 3,
                          Colors::Red);
      }
    }
    for (const auto monster : MONSTERS) {
      if (BoundCheckObject(monster, tile_x, tile_y)) continue;

      DrawSquareProFast(draw_x + (monster->tile_pos.x - tile_x) * zoom,
                        draw_y + (monster->tile_pos.y - tile_y) * zoom, zoom,
                        Colors::Red);
    }
    for (const auto npc : NPCS) {
      if (BoundCheckObject(npc, tile_x, tile_y)) continue;
      if (npc->id == NPC_ID::RANDOM || npc->id == NPC_ID::VILLAGER) {
        DrawSquareProFast(draw_x + (npc->tile_pos.x - tile_x) * zoom,
                          draw_y + (npc->tile_pos.y - tile_y) * zoom, zoom,
                          Colors::green_npc);
      } else {
        DrawSquareProFast(draw_x + (npc->tile_pos.x - tile_x) * zoom,
                          draw_y + (npc->tile_pos.y - tile_y) * zoom, zoom,
                          Colors::blue_npc);
      }
    }
  }

  void Update() noexcept {
    if (update_timer >= 50) {
      update_timer = 0;

      auto timePoint = GetGameTimePoint();
      ss.str("");
      ss.clear();

      ss << std::setw(2) << std::setfill('0') << timePoint.x << ":" << std::setw(2)
         << std::setfill('0') << timePoint.y;

      time_str = ss.str();
    }
    update_timer++;
  }
  inline static PointI GetGameTimePoint() noexcept {
    constexpr int ticksPerHour = Lighting::FULL_DAY_TICKS / 24;
    int hours = ((Lighting::dayTicks + ticksPerHour * 12) % Lighting::FULL_DAY_TICKS) / ticksPerHour;
    int minutes = ((Lighting::dayTicks + ticksPerHour * 12) % ticksPerHour) * 60 / ticksPerHour;
    return {hours % 24, minutes};
  }

};
#endif  //MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
