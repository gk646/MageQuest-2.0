#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_

struct WorldObject : public Entity {
  WorldObject(const Point& pos, const PointT<int16_t>& size, ShapeType shape_type,
              Zone zone)
      : Entity(pos, size, shape_type, 0, zone) {}

  inline static WorldObject* GetNewWorldObject(WorldObjectType type,
                                               const cxstructs::PointI& pos, bool isUsed,
                                               Zone zone);
};

//Interactive objects like containers, gates or doors
struct InteractableObject : public WorldObject {
  const std::vector<Texture>& textures;
  InteractableObject(std::vector<Texture>& textures, const Point& pos,
                     const PointT<int16_t> size, ShapeType type, bool isUsed, Zone zone)
      : WorldObject(pos, size, type, zone), isUsed(isUsed), textures(textures) {}
  bool isUsed = false;
  virtual void Interact() = 0;
  void Draw() final {
    if (!isUsed) {
      DrawTextureProFast(textures[0], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, WHITE);
    } else {
      int num = std::min(spriteCounter / 15, (int)textures.size() - 1);
      DrawTextureProFast(textures[num], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, WHITE);
    }
    DRAW_HITBOXES();
  };
  void Update() final {
    ENTITY_UPDATE()
    if (this->intersects(PLAYER) && Util::EPressed()) {
      isUsed = true;
      Interact();
    }
    if (isUsed && spriteCounter < 150) {
      spriteCounter++;
    }
  }
};

#include "worldobjects/DroppedItem.h"
#include "worldobjects/Chests.h"

//Returns a ptr to a new WorldObject of the given type at the given position
WorldObject* WorldObject::GetNewWorldObject(WorldObjectType type,
                                            const cxstructs::PointI& pos, bool isUsed,
                                            Zone zone) {
  switch (type) {
    case WorldObjectType::BASIC_CHEST:
      return new BasicChest(pos, isUsed, zone);
    case WorldObjectType::NORMAL_CHEST:
      return new NormalChest(pos, isUsed, zone);
    case WorldObjectType::EPIC_CHEST:
      return new EpicChest(pos, isUsed, zone);
  }
}

//Does a lookup in the database table if the object was already used
void Map::RegisterWorldObject(WorldObjectType type, const cxstructs::PointI& pos,
                              Zone zone) {
  bool isUsed =
      DataBaseHandler::StateExistsInTable("OBJECT_STATES", (int)type, pos.x, pos.y);
  WORLD_OBJECTS.emplace_back(WorldObject::GetNewWorldObject(type, pos, isUsed, zone));
}

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
