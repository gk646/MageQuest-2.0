#ifndef MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
#define MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_

//Draws the minimap at the top right
struct MiniMap {
  static constexpr int WIDTH = 200;
  static constexpr int HEIGHT = 200;
  static constexpr int ZOOM = 5;
  static constexpr float START_Y = 35;
  static constexpr int MINIMAP_TILE_WIDTH = WIDTH / ZOOM;
  std::stringstream ss{};
  std::string time_str{};
  int update_timer = 500;
  bool& isRegionMapOpen;
  explicit MiniMap(bool& isRegionMapOpen) : isRegionMapOpen(isRegionMapOpen) {}
  void Draw() const noexcept {
    auto playerTile = PLAYER.tilePos;
    float drawX = SCREEN_WIDTH - 25 - WIDTH;
    int tileX = playerTile.x - MINIMAP_TILE_WIDTH / 2;
    int tileY = playerTile.y - MINIMAP_TILE_WIDTH / 2;

    DrawTextExR(ANT_PARTY, zoneMap[CURRENT_ZONE].c_str(), {(float)drawX - 20, 9}, 17, 1,
                Colors::white);
    DrawTextExR(ANT_PARTY, time_str.c_str(), {SCREEN_WIDTH - 55, 9}, 17, 1,
                Colors::white);

    DrawRectangleLinesEx({(float)drawX - 3, START_Y - 3, WIDTH + 6, HEIGHT + 6}, 3,
                         Colors::LightGrey);

    if (isRegionMapOpen && FAST_UI) {
      return;
    }

    for (int i = 0; i < MINIMAP_TILE_WIDTH; i++) {
      for (int j = 0; j < MINIMAP_TILE_WIDTH; j++) {
        if (BoundCheckMap(tileX + i, tileY + j)) [[likely]] {
          if (IsTileCovered(tileX + i, tileY + j)) {
            DrawSquareProFast(drawX + i * ZOOM, START_Y + j * ZOOM, ZOOM, Colors::black);
            continue;
          }
          if (CheckTileCollision(tileX + i, tileY + j)) [[unlikely]] {
            DrawSquareProFast(drawX + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                              Colors::darkBackground);
          } else {
            if (tileX + i == playerTile.x && tileY + j == playerTile.y) [[unlikely]] {
              DrawSquareProFast(drawX + i * ZOOM, START_Y + j * ZOOM, ZOOM, Colors::Blue);

            } else {
              DrawSquareProFast(drawX + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                                Colors::map_green);
            }
          }
        } else {
          DrawSquareProFast(drawX + i * ZOOM, START_Y + j * ZOOM, ZOOM,
                            Colors::lightGreyMiddleAlpha);
        }
      }
    }

    if (FAST_UI) {
      return;
    }

    DrawExtras(tileX, tileY, drawX);
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
  //Draws monsters, player position, quest waypoint and npcs locations...
  static inline void DrawExtras(int tile_x, int tile_y, float draw_x) noexcept {
    for (const auto projectile : PROJECTILES) {
      if (BoundCheckObject(projectile, tile_x, tile_y)) continue;

      if (projectile->isFriendlyToPlayer) {
        DrawSquareProFast(draw_x + ((float)projectile->tilePos.x - (float)tile_x) * ZOOM,
                          START_Y + ((float)projectile->tilePos.y - (float)tile_y) * ZOOM,
                          3, Colors::Blue);
      } else {
        DrawSquareProFast(draw_x + ((float)projectile->tilePos.x - (float)tile_x) * ZOOM,
                          START_Y + ((float)projectile->tilePos.y - (float)tile_y) * ZOOM,
                          3, Colors::Red);
      }
    }
    for (const auto monster : MONSTERS) {
      if (!monster->isUpdated || BoundCheckObject(monster, tile_x, tile_y)) continue;
      DrawSquareProFast(draw_x + ((float)monster->tilePos.x - (float)tile_x) * ZOOM,
                        START_Y + ((float)monster->tilePos.y - (float)tile_y) * ZOOM,
                        ZOOM, Colors::Red);
    }
    for (const auto npc : NPCS) {
      if (!npc->isUpdated || BoundCheckObject(npc, tile_x, tile_y)) continue;
      if (npc->id == NPC_ID::RANDOM || npc->id == NPC_ID::VILLAGER) {
        DrawSquareProFast(draw_x + ((float)npc->tilePos.x - (float)tile_x) * ZOOM,
                          START_Y + ((float)npc->tilePos.y - (float)tile_y) * ZOOM, ZOOM,
                          Colors::green_npc);
      } else {
        DrawSquareProFast(draw_x + ((float)npc->tilePos.x - (float)tile_x) * ZOOM,
                          START_Y + ((float)npc->tilePos.y - (float)tile_y) * ZOOM, ZOOM,
                          Colors::blue_npc);
      }
    }
    PointT<int16_t> questWaypoint;
    if (PLAYER_QUESTS.activeQuest &&
        (questWaypoint = PLAYER_QUESTS.activeQuest->GetActiveWaypoint()) != 0 &&
        !BoundCheckPoint(questWaypoint, tile_x, tile_y)) {
      DrawSquareProFast(draw_x + ((float)questWaypoint.x - (float)tile_x) * ZOOM,
                        START_Y + ((float)questWaypoint.y - (float)tile_y) * ZOOM, ZOOM,
                        Colors::questMarkerYellow);
    }
  }
  //Returns a time point with the "x" being the hour and "y" being minutes in 24-hour format
  inline static PointI GetGameTimePoint() noexcept {
    constexpr int ticksPerHour = Lighting::FULL_DAY_TICKS / 24;
    int hours = ((Lighting::dayTicks + ticksPerHour * 12) % Lighting::FULL_DAY_TICKS) /
                ticksPerHour;
    int minutes =
        ((Lighting::dayTicks + ticksPerHour * 12) % ticksPerHour) * 60 / ticksPerHour;
    return {hours % 24, minutes};
  }
  static inline bool BoundCheckObject(const Entity* e, int tile_x, int tile_y) noexcept {
    return !e->isUpdated || e->tilePos.x < tile_x || e->tilePos.y < tile_y ||
           e->tilePos.x >= tile_x + MINIMAP_TILE_WIDTH ||
           e->tilePos.y >= tile_y + MINIMAP_TILE_WIDTH;
  }
  static inline bool BoundCheckPoint(const PointT<int16_t>& p, int tile_x,
                                     int tile_y) noexcept {
    return p.x < tile_x || p.y < tile_y || p.x >= tile_x + MINIMAP_TILE_WIDTH ||
           p.y >= tile_y + MINIMAP_TILE_WIDTH;
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_MINIMAP_H_
