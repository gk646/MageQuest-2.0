#ifndef MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_
#define MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_

#include "WorldAnimations.h"

struct WorldRender {
  static uint16_t worldCol, worldRow, maxCol, maxRow;
  static inline void PreRenderTasks() noexcept {
    worldCol = std::max(PLAYER_TILE->x - 21, 0);
    worldRow = std::max(PLAYER_TILE->y - 12, 0);
    maxCol = std::min(worldCol + 42, CURRENT_MAP_SIZE);
    maxRow = std::min(worldRow + 24, CURRENT_MAP_SIZE);
    const int playerX = PLAYER_X = PLAYER.pos.x_;
    const int playerY = PLAYER_Y = PLAYER.pos.y_;
    int screenX = CAMERA_X;
    int screenY = CAMERA_Y;
    const int worldWidth = CURRENT_MAP_SIZE * TILE_SIZE;

    if (screenX > playerX) {
      CAMERA_X = screenX = playerX;
    } else if (playerX > worldWidth - SCREEN_WIDTH / 2) {
      CAMERA_X = screenX = SCREEN_WIDTH - (worldWidth - playerX);
      worldCol = std::min(std::max(worldCol - 18, 0), CURRENT_MAP_SIZE);
    }

    if (screenY > playerY) {
      CAMERA_Y = screenY = playerY;
    } else if (playerY > worldWidth - SCREEN_HEIGHT / 2) {
      CAMERA_Y = screenY = SCREEN_HEIGHT - (worldWidth - playerY);
      worldRow = std::min(std::max(worldRow - 10, 0), CURRENT_MAP_SIZE);
    }

    Lighting::Shaders::cameraVec->x = CAMERA_X - 10;
    Lighting::Shaders::cameraVec->y = CAMERA_Y - 24;

    DRAW_X = -playerX + screenX;
    DRAW_Y = -playerY + screenY;
    MIRROR_POINT = playerX + PLAYER.size.x_ / 2;
  }
  static void DrawBackGround() noexcept {

    float x_base, y_base;
    for (uint16_t i = worldCol; i < maxCol; ++i) {
      x_base = i * TILE_SIZE + DRAW_X;
      for (uint16_t b = worldRow; b < maxRow; ++b) {
        y_base = b * TILE_SIZE + DRAW_Y;

        DrawTextureProFastUltra(TILES[CURRENT_BACK_GROUND[i][b]], x_base, y_base);

#ifdef DRAW_TILE_BORDER
        DrawRectangleLines(x_base, y_base, TILE_SIZE, TILE_SIZE, RED);
#endif
        int num2 = CURRENT_MIDDLE_GROUND[i][b];
        if (num2 != -1) {
          DrawTextureProFastUltra(TILES[num2], x_base, y_base);
        }
      }
    }
  }
  static void DrawForeGround() noexcept {
    float x_base, y_base;
    for (uint16_t i = worldCol; i < maxCol; ++i) {
      x_base = i * TILE_SIZE + DRAW_X;
      for (uint16_t b = worldRow; b < maxRow; ++b) {
        y_base = b * TILE_SIZE + DRAW_Y;

        int num1 = CURRENT_FORE_GROUND[i][b];
        if (num1 != -1) {
          DrawTextureProFastUltra(TILES[num1], x_base, y_base);
        }
      }
    }
  }
};
uint16_t WorldRender::worldRow = 0;
uint16_t WorldRender::worldCol = 0;
uint16_t WorldRender::maxCol = 0;
uint16_t WorldRender::maxRow = 0;

#endif  //MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_
