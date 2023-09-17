#ifndef MAGEQUEST_SRC_SYSTEM_COLLISIONDETECTION_H_
#define MAGEQUEST_SRC_SYSTEM_COLLISIONDETECTION_H_
namespace SAT {
Vector2 subtract(const Point& a, const Point& b) {
  return {a.x_ - b.x_, a.y_ - b.y_};
}

Vector2 normal(const Vector2& v) {
  return {-v.y, v.x};
}
inline static bool overlap(float minA, float maxA, float minB, float maxB) noexcept {
  return minB <= maxA && minA <= maxB;
}

std::array<Vector2, 4> getCorners(const Point& topLeft, float width, float height,
                                  float angle) {
  float halfWidth = width * 0.5f;
  float halfHeight = height * 0.5f;

  float sinRotation = sinf(angle * DEG2RAD);
  float cosRotation = cosf(angle * DEG2RAD);

  float topLeftX =
      topLeft.x_ + halfWidth - halfWidth * cosRotation + halfHeight * sinRotation;
  float topLeftY =
      topLeft.y_ + halfHeight - halfWidth * sinRotation - halfHeight * cosRotation;

  float topRightX =
      topLeft.x_ + halfWidth + halfWidth * cosRotation + halfHeight * sinRotation;
  float topRightY =
      topLeft.y_ + halfHeight + halfWidth * sinRotation - halfHeight * cosRotation;

  float bottomLeftX =
      topLeft.x_ + halfWidth - halfWidth * cosRotation - halfHeight * sinRotation;
  float bottomLeftY =
      topLeft.y_ + halfHeight - halfWidth * sinRotation + halfHeight * cosRotation;

  float bottomRightX =
      topLeft.x_ + halfWidth + halfWidth * cosRotation - halfHeight * sinRotation;
  float bottomRightY =
      topLeft.y_ + halfHeight + halfWidth * sinRotation + halfHeight * cosRotation;

  return {Vector2{topLeftX, topLeftY}, Vector2{topRightX, topRightY},
          Vector2{bottomLeftX, bottomLeftY}, Vector2{bottomRightX, bottomRightY}};
}

static bool rectanglesIntersect(const Point& topLeft1, float width1, float height1,
                                float angle1, const Point& topLeft2, float width2,
                                float height2, float angle2) noexcept {
  if (topLeft1.x_ + width1 < topLeft2.x_ || topLeft1.y_ + height1 < topLeft2.y_) {
    return false;
  }

  std::array<Vector2, 4> A = getCorners(topLeft1, width1, height1, angle1);
  std::array<Vector2, 4> B = getCorners(topLeft2, width2, height2, angle2);

  for (int a = 0; a < 4; a++) {
    int b = (a + 1) % 4;
    Point axisProj = {-A[b].y + A[a].y, A[b].x - A[a].x};

    float minA = axisProj.x_ * A[0].x + axisProj.y_ * A[0].y;
    float maxA = minA;
    for (int p = 1; p < 4; p++) {
      float t = axisProj.x_ * A[p].x + axisProj.y_ * A[p].y;
      minA = std::min(minA, t);
      maxA = std::max(maxA, t);
    }

    float minB = axisProj.x_ * B[0].x + axisProj.y_ * B[0].y;
    float maxB = minB;
    for (int p = 1; p < 4; p++) {
      float t = axisProj.x_ * B[p].x + axisProj.y_ * B[p].y;
      minB = std::min(minB, t);
      maxB = std::max(maxB, t);
    }

    if (!overlap(minA, maxA, minB, maxB)) {
      return false;
    }
  }

  for (int a = 0; a < 4; a++) {
    int b = (a + 1) % 4;
    Point axisProj = {-B[b].y + B[a].y, B[b].x - B[a].x};

    float minA = axisProj.x_ * A[0].x + axisProj.y_ * A[0].y;
    float maxA = minA;
    for (int p = 1; p < 4; p++) {
      float t = axisProj.x_ * A[p].x + axisProj.y_ * A[p].y;
      minA = std::min(minA, t);
      maxA = std::max(maxA, t);
    }

    float minB = axisProj.x_ * B[0].x + axisProj.y_ * B[0].y;
    float maxB = minB;
    for (int p = 1; p < 4; p++) {
      float t = axisProj.x_ * B[p].x + axisProj.y_ * B[p].y;
      minB = std::min(minB, t);
      maxB = std::max(maxB, t);
    }

    if (!overlap(minA, maxA, minB, maxB)) {
      return false;
    }
  }
  return true;
}
}  // namespace SAT
#endif  //MAGEQUEST_SRC_SYSTEM_COLLISIONDETECTION_H_
