#ifndef DND_SRC_ENTITY_H_
#define DND_SRC_ENTITY_H_

#include <string>
#include <utility>

#include "../../util/Enums.h"
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
    if(shape_type == ShapeType::RECT){
      if(o.shape_type == ShapeType::RECT){
        return (pos.x() < o.pos.x() + o.size.x() && pos.x() + size.x() > o.pos.x() &&
                pos.y() < o.pos.y() + o.size.y() && pos.y() + size.y() > o.pos.y());
      }else if(o.shape_type == ShapeType::CIRCLE){
        const float closestX = std::clamp(o.pos.x(), pos.x(), pos.x() + size.x());
        const float closestY = std::clamp(o.pos.y(), pos.y(), pos.y() + size.y());

        const float dx = closestX - o.pos.x();
        const float dy = closestY - o.pos.y();

        return (dx * dx + dy * dy) <= (o.size.x() * o.size.x());
      }
    }else if (shape_type == ShapeType::CIRCLE){
      if(o.shape_type == ShapeType::RECT){
        const float closestX = std::clamp(pos.x(), o.pos.x(), o.pos.x() + o.size.x());
        const float closestY = std::clamp(pos.y(), o.pos.y(), o.pos.y() + o.size.y());

        const float dx = closestX - pos.x();
        const float dy = closestY - pos.y();

        return (dx * dx + dy * dy) <= (size.x() * size.x());
      }else if (o.shape_type == ShapeType::CIRCLE){
        return (pos.x() - o.pos.x()) * (pos.x() - o.pos.x()) +
                   (pos.y() - o.pos.y()) * (pos.y() - o.pos.y()) <=
               (size.x() + o.size.x()) * (size.x() + o.size.x());
      }
    }
  }
  virtual void draw_hitbox(){};
  virtual void update(){

  };
};
#endif  //DND_SRC_ENTITY_H_
