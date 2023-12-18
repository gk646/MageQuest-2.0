#ifndef MAGEQUEST_SRC_GAMEPLAY_AOEINDICATOR_H_
#define MAGEQUEST_SRC_GAMEPLAY_AOEINDICATOR_H_

struct AoEIndicator : WorldObject {
  int16_t delayTicks;
  int16_t upCounter = 0;
  int16_t currentRadius = 0;
  AoEIndicator(const Point& pos, const PointT<int16_t>& size, ShapeType shapeType,
               Zone zone, int16_t delay)
      : WorldObject(pos, size, shapeType, zone), delayTicks(delay) {}

  void Draw() final {
    if (hitboxShape == ShapeType::RECT) {

    } else {
      DrawCircle(pos.x() +DRAW_X, pos.y() +DRAW_Y, currentRadius, RED);
      DrawCircleLines(pos.x() +DRAW_X, pos.y() +DRAW_Y, size.x,  RED);
    }
  }
  void Update() final {
    upCounter++;
    if (currentRadius >= size.x) isDead = true;
    currentRadius = (upCounter / (float)delayTicks) * size.x;
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_AOEINDICATOR_H_
