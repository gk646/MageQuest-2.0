#ifndef DND_SRC_ENTITY_H_
#define DND_SRC_ENTITY_H_

#include <string>
#include <utility>

#include "../../enums/Enums.h"
#include "attributes/Stats.h"
#include "cxstructs/Geometry.h"

using namespace cxstructs;

struct Entity {
  ShapeType shape_type;

  Point pos;
  Point size;
  Entity() : pos{}, size{}, shape_type(ShapeType::RECT) {}
  Entity(const Point& pos, const Point& size, ShapeType shape_type)
      : pos(pos), size(size), shape_type(shape_type) {}
  Entity(const Entity& o) : pos(o.pos), size(o.size), shape_type(o.shape_type) {}
  Entity& operator=(const Entity& other) {
    if (this == &other) {
      return *this;
    }

    pos = other.pos;
    size = other.size;
    shape_type = other.shape_type;

    return *this;
  }
  virtual ~Entity() {}
  virtual void draw(){};
  [[nodiscard]] bool intersects(const Entity& o) const {
    switch (o.shape_type) {
      case RECT:
        switch (shape_type) {
          case RECT: {
            return (pos.x() < o.pos.x() + o.size.x() && pos.x() + size.x() > o.pos.x() &&
                    pos.y() < o.pos.y() + o.size.y() && pos.y() + size.y() > o.pos.y());
          }
          case CIRCLE: {
            const float closestX = std::clamp(o.pos.x(), pos.x(), pos.x() + size.x());
            const float closestY = std::clamp(o.pos.y(), pos.y(), pos.y() + size.y());

            const float dx = closestX - o.pos.x();
            const float dy = closestY - o.pos.y();

            return (dx * dx + dy * dy) <= (o.size.x() * o.size.x());
          }

        }
        break;
      case CIRCLE:
        switch (shape_type) {
          case RECT: {
            return (pos.x() < o.pos.x() + o.size.x() && pos.x() + size.x() > o.pos.x() &&
                    pos.y() < o.pos.y() + o.size.y() && pos.y() + size.y() > o.pos.y());
          }
          case CIRCLE: {
            const float closestX = std::clamp(o.pos.x(), pos.x(), pos.x() + size.x());
            const float closestY = std::clamp(o.pos.y(), pos.y(), pos.y() + size.y());

            return ((closestX - o.pos.x()) * (closestX - o.pos.x()) +
                    (closestY - o.pos.y()) * (closestY - o.pos.y())) <= (o.size.x() * o.size.x());
          }
        }
    }
  }
  virtual void draw_hitbox(){};
  virtual void update(){

  };
};
#endif  //DND_SRC_ENTITY_H_
