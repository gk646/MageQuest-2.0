#ifndef MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_
#define MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_

struct WorldRender {
  static int player_x;
  static int player_y;
  static void draw() noexcept {
    int worldCol = std::max(PLAYER_TILE_X - 21, 0);
    int worldRow = std::max(PLAYER_TILE_Y - 12, 0);
    int maxCol = std::min(worldCol + 42, CURRENT_MAP_SIZE);
    int maxRow = std::min(worldRow + 25, CURRENT_MAP_SIZE);

    int playerX = PLAYER_X = PLAYER.pos.x();
    int playerY = PLAYER_Y = PLAYER.pos.y();
    int screenX = CAMERA_X;
    int screenY = CAMERA_Y;
    int worldWidth = CURRENT_MAP_SIZE * TILE_SIZE;

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

    player_x = -playerX + screenX;
    player_y = -playerY + screenY;

    float x_base, y_base;
    for (uint_fast16_t i = worldCol; i < maxCol; ++i) {
      x_base = i * TILE_SIZE + player_x;
      for (uint_fast16_t b = worldRow; b < maxRow; ++b) {
        y_base = b * TILE_SIZE + player_y;

        DrawTextureProFast(TEXTURES[CURRENT_BACK_GROUND[i][b]], x_base, y_base,
                           0, WHITE);

        int num2 = CURRENT_MIDDLE_GROUND[i][b];
        if (num2 != -1) {
          DrawTextureProFast(TEXTURES[num2], x_base, y_base, 0, WHITE);
        }
      }
    }
  }
  static void draw_fore_ground() noexcept {
    int worldCol = std::max(PLAYER_TILE_X - 21, 0);
    int worldRow = std::max(PLAYER_TILE_Y - 12, 0);
    int maxCol = std::min(worldCol + 42, CURRENT_MAP_SIZE);
    int maxRow = std::min(worldRow + 25, CURRENT_MAP_SIZE);

    float x_base, y_base;
    for (uint_fast16_t i = worldCol; i < maxCol; ++i) {
      x_base = i * TILE_SIZE + player_x;
      for (uint_fast16_t b = worldRow; b < maxRow; ++b) {
        y_base = b * TILE_SIZE + player_y;

        int num1 = CURRENT_FORE_GROUND[i][b];
        if (num1 != -1) {
          DrawTextureProFast(TEXTURES[num1], x_base, y_base, 0, WHITE);
        }
      }
    }
  }
};
int WorldRender::player_x = 0;
int WorldRender::player_y = 0;
#endif  //MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_
