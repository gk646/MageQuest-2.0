#ifndef MAGEQUEST_SRC_UI_ELEMENTS_UIHITBOX_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_UIHITBOX_H_
struct UIHitbox {
  RectangleR bounds;
  bool isHovered;
  bool wasClicked;
  UIHitbox(float width, float height) : bounds(0, 0, width, height) {}

  inline bool Update(float x, float y) {
    bounds.x = x;
    bounds.y = y;
    isHovered = CheckCollisionPointRec(MOUSE_POS, bounds);
    return isHovered;
  }

  inline bool IsHovered() const noexcept {}
  inline bool WasClicked() const noexcept {}
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_UIHITBOX_H_
