#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_

struct WorldObject : public Entity {

  WorldObject(const Point& pos, const Point& size, ShapeType shape_type)
      : Entity(pos, size, shape_type) {}

};
#include "worldobjects/DroppedItem.h"
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
