#ifndef MAGEQUEST_SRC_GRAPHICS_SCREENEFFECTS_H_
#define MAGEQUEST_SRC_GRAPHICS_SCREENEFFECTS_H_
namespace ScreenEffects {
inline static constexpr uint16_t FULL_DAY_TICKS = 15 * 60 * 60;
inline static uint16_t dayTicks = 0;
inline static Color currentDayFade{24, 20, 37, 0};
inline static uint8_t fadeAlpha = 150;

uint8_t CalculateAlpha() {
  double phaseOfDay = (2 * PI * dayTicks) / FULL_DAY_TICKS;
  double alphaDouble = (std::sin(phaseOfDay - (PI / 2)) + 1) / 2;
  return static_cast<uint8_t>(210 * alphaDouble);
}

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
  dayTicks = (dayTicks + 1) % FULL_DAY_TICKS;
  currentDayFade.a = CalculateAlpha();
}
}  // namespace ScreenEffects
#endif  //MAGEQUEST_SRC_GRAPHICS_SCREENEFFECTS_H_
