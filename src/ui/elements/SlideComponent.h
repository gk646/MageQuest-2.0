#ifndef MAGEQUEST_SRC_UI_ELEMENTS_SLIDECOMPONENT_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_SLIDECOMPONENT_H_
struct SlideComponent {
  const bool& trigger;
  float maxTick, speed, currentTick;
  SlideComponent(const bool& trigger, float maxAnimationTick, float speed)
      : maxTick(maxAnimationTick), speed(speed), trigger(trigger) {}

  inline void UpdateAnimation() noexcept {
    if (trigger) {
      if (currentTick < maxTick) {
        currentTick += speed;
        if (currentTick > maxTick) {
          currentTick = maxTick;
        }
      }
    } else {
      if (currentTick > 0) {
        currentTick -= speed;
        if (currentTick < 0) {
          currentTick = 0;
        }
      }
    }
  }
  [[nodiscard]] inline float GetCurrentOffset() const noexcept { return currentTick; }
  [[nodiscard]] inline bool IsAnimationFinished() const noexcept {
    return currentTick == 0;
  }
  inline void Reset() noexcept { currentTick = 0; }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_SLIDECOMPONENT_H_
