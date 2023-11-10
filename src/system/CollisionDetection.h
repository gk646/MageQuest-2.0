#ifndef MAGEQUEST_SRC_SYSTEM_COLLISIONDETECTION_H_
#define MAGEQUEST_SRC_SYSTEM_COLLISIONDETECTION_H_
namespace SAT {

inline Vector2 Subtract(const Vector2& a, const Vector2& b) {
  return {a.x - b.x, a.y - b.y};
}

inline Vector2 Perpendicular(const Vector2& v) {
  return {-v.y, v.x};
}

inline bool Overlap(float minA, float maxA, float minB, float maxB) noexcept {
  return minB <= maxA && minA <= maxB;
}

std::array<Vector2, 4> GetCorners(const Point& center, float width, float height,
                                  float angle) {
  if (angle == 0.0f) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    return {Vector2{center.x_ - halfWidth, center.y_ - halfHeight},
            Vector2{center.x_ + halfWidth, center.y_ - halfHeight},
            Vector2{center.x_ - halfWidth, center.y_ + halfHeight},
            Vector2{center.x_ + halfWidth, center.y_ + halfHeight}};
  }

  std::array<Vector2, 4> corners{};
  float sinAngle = std::sin(angle * DEG2RAD);
  float cosAngle = std::cos(angle * DEG2RAD);
  float halfWidth = width / 2.0f;
  float halfHeight = height / 2.0f;

  std::array<Vector2, 4> relativeCorners = {
      Vector2{-halfWidth, -halfHeight}, Vector2{halfWidth, -halfHeight},
      Vector2{-halfWidth, halfHeight}, Vector2{halfWidth, halfHeight}};

  for (int i = 0; i < 4; ++i) {
    corners[i] = {
        center.x_ + (relativeCorners[i].x * cosAngle - relativeCorners[i].y * sinAngle),
        center.y_ + (relativeCorners[i].x * sinAngle + relativeCorners[i].y * cosAngle)};
  }

  return corners;
}

bool ProjectAndCheckOverlap(const std::array<Vector2, 4>& cornersA,
                            const std::array<Vector2, 4>& cornersB, const Vector2& axis) {
  float minA = FLT_MAX;
  float maxA = FLT_MIN;
  float minB = FLT_MAX;
  float maxB = FLT_MIN;

  for (const auto& corner : cornersA) {
    float projection = corner.x * axis.x + corner.y * axis.y;
    minA = std::min(minA, projection);
    maxA = std::max(maxA, projection);
  }

  for (const auto& corner : cornersB) {
    float projection = corner.x * axis.x + corner.y * axis.y;
    minB = std::min(minB, projection);
    maxB = std::max(maxB, projection);
  }

  return Overlap(minA, maxA, minB, maxB);
}

inline bool RectanglesIntersect(const Point& pos1, float width1, float height1,
                                float angle1, const Point& pos2, float width2,
                                float height2, float angle2) noexcept {
  if (pos1.x_ + width1 < pos2.x_ || pos1.y_ + height1 < pos2.y_ ||
      pos2.x_ + width2 < pos1.x_ || pos2.y_ + height2 < pos1.y_) {
    return false;
  }

  auto cornersA = GetCorners(pos1, width1, height1, angle1);
  auto cornersB = GetCorners(pos2, width2, height2, angle2);

  for (int i = 0; i < 4; i++) {
    Vector2 edge = Subtract(cornersA[(i + 1) % 4], cornersA[i]);
    Vector2 axis = Perpendicular(edge);
    if (!ProjectAndCheckOverlap(cornersA, cornersB, axis)) {
      return false;
    }
  }

  for (int i = 0; i < 4; i++) {
    Vector2 edge = Subtract(cornersB[(i + 1) % 4], cornersB[i]);
    Vector2 axis = Perpendicular(edge);
    if (!ProjectAndCheckOverlap(cornersA, cornersB, axis)) {
      return false;
    }
  }

  return true;
}

}  // namespace SAT

#endif  //MAGEQUEST_SRC_SYSTEM_COLLISIONDETECTION_H_
