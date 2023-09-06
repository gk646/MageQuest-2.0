#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_

struct DroppedItem final : public WorldObject {
  Item* item;
  DroppedItem(const Point& pos, Item* item) : WorldObject(pos, {45, 45}, ShapeType::RECT), item(item) {}

  void update()final{

  }
  void draw() final{
    DrawRectanglePro(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(),
                     size.y(), {0, 0}, pov, PINK);
  }
  void collision() final {
    CharacterBag::add_item(item);
    item = nullptr;
    dead = true;
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
