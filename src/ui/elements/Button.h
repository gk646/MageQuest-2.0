#ifndef MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
struct Button {
  RectangleR bounds;
  float base_width, base_height;
  bool entered = false;
  const char* txt = nullptr;
  float fontSize = 15;
  const Texture& normal;
  const Texture& hovered;
  const Texture& pressed;
  Button(float width, float height, const char* txt, float fontSize,const Texture& normal,const Texture& hovered,const Texture& pressed)
      : bounds(0, 0, width, height),
        base_width(width),
        base_height(height),
        txt(txt),
        fontSize(fontSize), normal(normal), hovered(hovered), pressed(pressed){}
  bool Draw(float x, float y, bool* hoverStatus = nullptr) noexcept {
    Update(x, y);
    if (CheckCollisionPointRec(MOUSE_POS, bounds)) {
      PlayerEnterSound();
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        DrawTextureScaled(pressed, bounds, 0, false, 0, WHITE);
      } else {
        if(hoverStatus) *hoverStatus = true;
        DrawTextureScaled(hovered, bounds, 0, false, 0, WHITE);
      }
    } else {
      DrawTextureScaled(normal, bounds, 0, false, 0, WHITE);
      entered = false;
    }
    DrawCenteredText(VARNISHED, fontSize, txt, bounds.x + bounds.width / 2.0F,
                     bounds.y + bounds.height / 3.4F, GetTextColor());
    return PlayConfirmSound();
  }

 private:
  [[nodiscard]] inline const Color& GetTextColor() const noexcept {
    if (entered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      return Colors::LightGrey;
    }
    return Colors::darkBackground;
  }
  inline void Update(float x, float y) {
    bounds.x = x;
    bounds.y = y;
    bounds.width = SCALE(base_width);
    bounds.height = SCALE(base_height);
    bounds.x -= bounds.width / 2;
  }
  inline void PlayerEnterSound() {
    if (!entered) {
      PlaySoundR(sound::menu_switch);
      entered = true;
    }
  }
  [[nodiscard]] inline bool PlayConfirmSound() const noexcept {
    if (entered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      PlaySoundR(sound::menu_switch);
      return true;
    }
    return false;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
