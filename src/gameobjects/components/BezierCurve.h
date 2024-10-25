#ifndef MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_BEZIERCURVE_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_BEZIERCURVE_H_

struct BezierCurve {
  Point start;
  Point control;
  Point end;

  BezierCurve(const Point& start, const Point& control, const Point& end)
      : start(start), control(control), end(end) {}

  Point CalculatePoint(float t) const {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;

    Point p = start * uu;
//    p += control * t * u * 2;
  //  p += end * tt;

    return p;
  }

  static Point CalculateControlPoint(const Point& start, const Point& end) {
    return {(start.x() + end.x()) / 2, (start.y() + end.y()) / 2 - 100};
  }
};


#endif  //MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_BEZIERCURVE_H_
