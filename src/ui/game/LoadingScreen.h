#ifndef MAGE_QUEST_SRC_LOADING_LOADINGSCREEN_H_
#define MAGE_QUEST_SRC_LOADING_LOADINGSCREEN_H_

struct LoadingScreen {
  static constexpr float beginX = 0.65;
  static constexpr float beginY = 0.90;
  static constexpr float width = 0.30;
  static constexpr float height = 0.020;

  static constexpr float vertical_gap = 0.0020;
  static constexpr float horizontal_gap = 0.0012;
  static float progress;
  static void draw() noexcept {
    DrawRectanglePro(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, 0, Colors::LightGrey);

    DrawRectangleLines(SCREEN_WIDTH * beginX, SCREEN_HEIGHT * beginY,
                       SCREEN_WIDTH * width, SCREEN_HEIGHT * height,
                       Colors::darkBackground);

    DrawRectanglePro(SCREEN_WIDTH * beginX + SCREEN_WIDTH * horizontal_gap,
                     SCREEN_HEIGHT * beginY + SCREEN_HEIGHT * vertical_gap,
                     SCREEN_WIDTH * (progress / 100) * (width - horizontal_gap * 2),
                     SCREEN_HEIGHT * (height - vertical_gap * 2), {0, 0}, 0,
                     Colors::darkBackground);

    if (RAYLIB_LOGO) {
      RAYLIB_LOGO->draw();
      if (RAYLIB_LOGO->finished && NBNET_LOGO) {
        NBNET_LOGO->draw();
      }
    }
  }
};

float LoadingScreen::progress = 0.0f;
#endif  //MAGE_QUEST_SRC_LOADING_LOADINGSCREEN_H_
