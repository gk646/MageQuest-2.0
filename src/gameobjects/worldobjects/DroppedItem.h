#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_

struct DroppedItem final : public WorldObject {
  Item* item;
  DroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {40, 40}, ShapeType::RECT), item(item) {

  }

  void update() final {}
  void draw() final {
    DrawTextureScaled(item->texture,{pos.x_+DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_}, 0, false, 0, WHITE);

  }
  void collision() final {
   dead =  CharacterBag::AddItem(item);
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
