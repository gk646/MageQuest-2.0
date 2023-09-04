#ifndef MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_
#define MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_

struct WorldRender {
  static void draw() noexcept {
    cxstructs::now();
    int worldCol = std::max(PLAYER_TILE_X - 21, 0);
    int worldRow = std::max(PLAYER_TILE_Y - 12, 0);
    int maxCol = std::min(worldCol + 42, CURRENT_MAP_SIZE);
    int maxRow = std::min(worldRow + 25, CURRENT_MAP_SIZE);

    int playerX = PLAYER_X;
    int playerY = PLAYER_Y;
    int screenX = CAMERA_X;
    int screenY = CAMERA_Y;
    int worldWidth = CURRENT_MAP_SIZE * TILE_SIZE;

    if (screenX > playerX) {
      CAMERA_X = screenX = playerX;
    } else if (playerX + 24 > worldWidth - SCREEN_WIDTH / 2) {
      CAMERA_X = screenX = SCREEN_WIDTH - (worldWidth - playerX);
      worldCol = std::min(std::max(worldCol - 18, 0), CURRENT_MAP_SIZE);
    }

    if (screenY > playerY) {
      CAMERA_Y = screenY = playerY;
    } else if (playerY + 24 > worldWidth - SCREEN_HEIGHT / 2) {
      CAMERA_Y = screenY = SCREEN_HEIGHT - (worldWidth - playerY);
      worldRow = std::min(std::max(worldRow - 10, 0), CURRENT_MAP_SIZE);
    }

    for (int i = worldCol; i < maxCol; ++i) {
      float x_base = i * TILE_SIZE - playerX + screenX;
      for (int b = worldRow; b < maxRow; ++b) {
        float y_base = b * TILE_SIZE - playerY + screenY;

        int num1 = CURRENT_BACK_GROUND[i][b];
        DrawTextureProFast(TEXTURES[num1],x_base,y_base,0,WHITE);

        int num2 = CURRENT_MIDDLE_GROUND[i][b];
        if (num2 != -1) {
          DrawTextureProFast(TEXTURES[num2],x_base,y_base,0,WHITE);
        }
      }
    }
    PERF_FRAMES++;
    PERF_TIME += cxstructs::getTime<std::chrono::nanoseconds>();
    //std::cout<< PERF_TIME/PERF_FRAMES << std::endl;
  }
  static void draw_fore_ground() noexcept {
    int worldCol = std::max(PLAYER_TILE_X - 21, 0);
    int worldRow = std::max(PLAYER_TILE_Y - 12, 0);
    int maxCol = std::min(worldCol + 42, CURRENT_MAP_SIZE);
    int maxRow = std::min(worldRow + 25, CURRENT_MAP_SIZE);

    int playerX = PLAYER_X;
    int playerY = PLAYER_Y;
    int screenX = CAMERA_X;
    int screenY = CAMERA_Y;
    int worldWidth = CURRENT_MAP_SIZE * TILE_SIZE;

    if (screenX > playerX) {
      CAMERA_X = screenX = playerX;
    } else if (playerX + 24 > worldWidth - SCREEN_WIDTH / 2) {
      CAMERA_X = screenX = SCREEN_WIDTH - (worldWidth - playerX);
      worldCol = std::min(std::max(worldCol - 18, 0), CURRENT_MAP_SIZE);
    }

    if (screenY > playerY) {
      CAMERA_Y = screenY = playerY;
    } else if (playerY + 24 > worldWidth - SCREEN_HEIGHT / 2) {
      CAMERA_Y = screenY = SCREEN_HEIGHT - (worldWidth - playerY);
      worldRow = std::min(std::max(worldRow - 10, 0), CURRENT_MAP_SIZE);
    }

    for (int i = worldCol; i < maxCol; ++i) {
      float x_base = i * TILE_SIZE - playerX + screenX;
      for (int b = worldRow; b < maxRow; ++b) {
        float y_base = b * TILE_SIZE - playerY + screenY;

        int num1 = CURRENT_FORE_GROUND[i][b];
        if(num1 != -1){
          DrawTextureProFast(TEXTURES[num1],x_base,y_base,0,WHITE);
        }

      }
    }
  }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_WORLDRENDER_H_
