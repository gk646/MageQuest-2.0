#ifndef DUNGEONM_SRC_LOADING_LOADINGSCREEN_H_
#define DUNGEONM_SRC_LOADING_LOADINGSCREEN_H_

#include "../../gamestateio/GameLoader.h"
#include "../../gamestateio/LoadingUtil.h"

struct LoadingScreen {
  const float beginX = 0.65;
  const float beginY = 0.93;
  const float width = 0.30;
  const float height = 0.020;

  const float vertical_gap = 0.0022;
  const float horizontal_gap = 0.0020;
  static float progress;

  void draw() noexcept {
    DrawRectanglePro(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, 0, Colors::mediumVeryLight);

    DrawRectangleLines(SCREEN_WIDTH * beginX, SCREEN_HEIGHT * beginY, SCREEN_WIDTH * width,
                       SCREEN_HEIGHT * height, Colors::gray);

    DrawRectanglePro(SCREEN_WIDTH * beginX + SCREEN_WIDTH * horizontal_gap,
                     SCREEN_HEIGHT * beginY + SCREEN_HEIGHT * vertical_gap,
                     SCREEN_WIDTH * (progress / 100) * (width - horizontal_gap * 2),
                     SCREEN_HEIGHT * (height - vertical_gap * 2), {0, 0}, 0, Colors::gray);
  }
};

float LoadingScreen::progress = 0.0f;
#endif  //DUNGEONM_SRC_LOADING_LOADINGSCREEN_H_
