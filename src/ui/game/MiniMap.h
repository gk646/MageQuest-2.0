#ifndef MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
#define MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_

struct MiniMap {
  static constexpr int WIDTH = 200;
  static constexpr int HEIGHT = 200;
  static constexpr int ZOOM = 5;
  static constexpr float START_Y = 35;
  static constexpr int MINIMAP_TILE_WIDTH = WIDTH / ZOOM;
  std::stringstream ss{};
  std::string time_str{};
  int update_timer = 500;
  bool* region_map_open;
  explicit MiniMap(bool* region_map_status) : region_map_open(region_map_status) {}
  static inline bool BoundCheckObject(const Entity* e, int tile_x, int tile_y) noexcept {
    return !e->isUpdated || e->tile_pos.x < tile_x || e->tile_pos.y < tile_y ||
           e->tile_pos.x >= tile_x + MINIMAP_TILE_WIDTH ||
           e->tile_pos.y >= tile_y + MINIMAP_TILE_WIDTH;
  }
  void Draw() const noexcept {
    auto player_tile = PLAYER.tile_pos;
    float draw_x = SCREEN_WIDTH - 25 - WIDTH;
    int tile_x = player_tile.x - MINIMAP_TILE_WIDTH / 2;
    int tile_y = player_tile.y - MINIMAP_TILE_WIDTH / 2;

    DrawTextExR(ANT_PARTY, zoneMap[CURRENT_ZONE].c_str(), {(float)draw_x - 20, 9}, 17, 1,
                Colors::white);
    DrawTextExR(ANT_PARTY, time_str.c_str(), {SCREEN_WIDTH - 55, 9}, 17, 1,
                Colors::white);

    DrawRectangleLinesEx({(float)draw_x - 3, START_Y - 3, WIDTH + 6, HEIGHT + 6}, 3,
                         Colors::LightGrey);
    if (*region_map_open && FAST_UI) {
      return;
    }

    for (int_fast32_t i = 0; i < MINIMAP_TILE_WIDTH; i++) {

      for (int_fast32_t j = 0; j < MINIMAP_TILE_WIDTH; j++) {
        if (BoundCheckMap(tile_x + i, tile_y + j)) [[likely]] {
          if (IsTileCovered(tile_x + i, tile_y + j)) {
            DrawSquareProFast(draw_x + i * ZOOM, START_Y + j * ZOOM, ZOOM, Colors::black);
            continue;
          }
          if (CheckTileCollision(tile_x + i, tile_y + j)) [[unlikely]] {
            DrawSquareProFast(draw_x + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                              Colors::darkBackground);
          } else {
            if (tile_x + i == player_tile.x && tile_y + j == player_tile.y) [[unlikely]] {
              DrawSquareProFast(draw_x + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                                Colors::Blue);

            } else {
              DrawSquareProFast(draw_x + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                                Colors::map_green);
            }
          }
        } else {
          DrawSquareProFast(draw_x + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                            Colors::lightGreyMiddleAlpha);
        }
      }
    }

    if (FAST_UI) {
      return;
    }

    DrawExtras(tile_x, tile_y, draw_x);
  }
  void Update() noexcept {
    if (update_timer >= 45) {
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

 private:
  static inline void DrawExtras(int tile_x, int tile_y, float draw_x) noexcept {
    for (const auto projectile : PROJECTILES) {
      if (BoundCheckObject(projectile, tile_x, tile_y)) continue;

      if (projectile->from_player) {
        DrawSquareProFast(draw_x + (projectile->tile_pos.x - tile_x) * ZOOM,
                          START_Y + (projectile->tile_pos.y - tile_y) * ZOOM, 3,
                          Colors::Blue);
      } else {
        DrawSquareProFast(draw_x + (projectile->tile_pos.x - tile_x) * ZOOM,
                          START_Y + (projectile->tile_pos.y - tile_y) * ZOOM, 3,
                          Colors::Red);
      }
    }
    for (const auto monster : MONSTERS) {
      if (!monster->isUpdated || BoundCheckObject(monster, tile_x, tile_y)) continue;
      DrawSquareProFast(draw_x + (monster->tile_pos.x - tile_x) * ZOOM,
                        START_Y + (monster->tile_pos.y - tile_y) * ZOOM, ZOOM,
                        Colors::Red);
    }
    for (const auto npc : NPCS) {
      if (!npc->isUpdated || BoundCheckObject(npc, tile_x, tile_y)) continue;
      if (npc->id == NPC_ID::RANDOM || npc->id == NPC_ID::VILLAGER) {
        DrawSquareProFast(draw_x + (npc->tile_pos.x - tile_x) * ZOOM,
                          START_Y + (npc->tile_pos.y - tile_y) * ZOOM, ZOOM,
                          Colors::green_npc);
      } else {
        DrawSquareProFast(draw_x + (npc->tile_pos.x - tile_x) * ZOOM,
                          START_Y + (npc->tile_pos.y - tile_y) * ZOOM, ZOOM,
                          Colors::blue_npc);
      }
    }
    PointT<int16_t> questWaypoint;
    if (PLAYER_QUESTS.activeQuest &&
        (questWaypoint = PLAYER_QUESTS.activeQuest->GetActiveWaypoint()) != 0) {
      DrawSquareProFast(draw_x + (questWaypoint.x - tile_x) * ZOOM,
                        START_Y + (questWaypoint.y - tile_y) * ZOOM, ZOOM,
                        Colors::questMarkerYellow);
    }
  }
  inline static PointI GetGameTimePoint() noexcept {
    constexpr int ticksPerHour = Lighting::FULL_DAY_TICKS / 24;
    int hours = ((Lighting::dayTicks + ticksPerHour * 12) % Lighting::FULL_DAY_TICKS) /
                ticksPerHour;
    int minutes =
        ((Lighting::dayTicks + ticksPerHour * 12) % ticksPerHour) * 60 / ticksPerHour;
    return {hours % 24, minutes};
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
