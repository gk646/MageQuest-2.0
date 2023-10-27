#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_

struct WorldObject : public Entity {

  WorldObject(const Point& pos, const PointT<int16_t>& size, ShapeType shape_type)
      : Entity(pos, size, shape_type) {}
};

struct InteractableObject : public WorldObject {
  bool interacted;
  virtual void Interact() = 0;
  void Draw() final {}
  void Update() final {}
};

#include "worldobjects/DroppedItem.h"
#include "worldobjects/Chests.h"
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
