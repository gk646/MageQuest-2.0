#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_

struct WorldObject : public Entity {
  WorldObject(const Point& pos, const PointT<int16_t>& size, ShapeType shape_type,
              Zone zone)
      : Entity(pos, size, shape_type, 0, false,zone) {}

  inline static WorldObject* GetNewInteractableObject(WorldObjectType type,
                                               const cxstructs::PointI& pos, bool isUsed,
                                               Zone zone);
};

//Interactive objects like containers, gates or doors
struct InteractableObject : public WorldObject {
  Sound sound;
  const std::vector<Texture>& textures;
  WorldObjectType type;
  bool isUsed = false;
  InteractableObject(std::vector<Texture>& textures, const Sound& sound, const Point& pos,
                     const PointT<int16_t> size, ShapeType shapeType, bool isUsed,
                     Zone zone, WorldObjectType objectType)
      : WorldObject(pos, size, shapeType, zone),
        isUsed(isUsed),
        textures(textures),
        type(objectType),
        sound(LoadSoundAlias(sound)) {
    if (isUsed) {
      SetSoundVolume(sound, 0);
      spriteCounter = 150;
    }
  }
  ~InteractableObject() override { UnloadSoundAlias(sound); }
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
    if (!isUsed && Util::EPressed() && this->Intersects(PLAYER)) {
      isUsed = true;
      Interact();
      PlaySoundR(sound);
      DataBaseHandler::AddStateToTable("OBJECT_STATES", (int)type, tilePos.x, tilePos.y,
                                       (int)currentZone);
    }
    if (isUsed && spriteCounter < 150) {
      spriteCounter++;
    }
  }
};

#include "worldobjects/DroppedItem.h"
#include "worldobjects/AoEIndicator.h"
#include "worldobjects/Chests.h"

//Returns a ptr to a new WorldObject of the given type at the given position
WorldObject* WorldObject::GetNewInteractableObject(WorldObjectType type,
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
void Map::RegisterInteractableObject(WorldObjectType type, const cxstructs::PointI& pos,
                              Zone zone) {
  bool isUsed = DataBaseHandler::StateExistsInTable("OBJECT_STATES", (int)type, pos.x,
                                                    pos.y, (int)zone);
  WORLD_OBJECTS.emplace_back(
      WorldObject::GetNewInteractableObject(type, pos, isUsed, zone));
}

void ItemDropHandler::DropItem(float x, float y, Item* item) noexcept {
  WORLD_OBJECTS.push_back(new DroppedItem({x, y}, item));
}

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECT_H_
