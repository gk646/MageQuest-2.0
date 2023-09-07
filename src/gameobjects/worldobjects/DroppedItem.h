#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_

struct DroppedItem final : public WorldObject {
  Item* item;
  DroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {45, 45}, ShapeType::RECT), item(item) {}

  void update() final {}
  void draw() final {
    DrawTextureProFast(item->texture, pos.x() + DRAW_X, pos.y() + DRAW_Y, 0,
                       WHITE);
  }
  void collision() final {
    CharacterBag::add_item(item);
    dead = true;
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
