#ifndef MAGEQUEST_SRC_GRAPHICS_SCREENEFFECTS_H_
#define MAGEQUEST_SRC_GRAPHICS_SCREENEFFECTS_H_
namespace ScreenEffects {
inline static constexpr uint16_t FULL_DAY_TICKS = 15 * 60 * 60;
inline static uint16_t dayTicks = 0;
inline static uint8_t fadeAlpha = 255;

inline static void DrawFade() noexcept {
  DrawRectangleProFast(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {24, 20, 37, fadeAlpha});
}
inline static void DrawScreenEffects() noexcept {
  if (fadeAlpha < 255) {
    DrawFade();
  }
}
inline static void UpdateScreenEffects() noexcept {
  if (fadeAlpha < 255) {
    fadeAlpha--;
  }
}

}  // namespace ScreenEffects
#endif  //MAGEQUEST_SRC_GRAPHICS_SCREENEFFECTS_H_
