#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_

struct DroppedItem final : public WorldObject {
  Item* item;
  DroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {40, 40}, ShapeType::RECT), item(item) {}

  void Update() final {
    ENTITY_UPDATE();
    if (this->intersects(PLAYER)) {
      isDead = CharacterBag::AddItem(item);
    }
  }
  void Draw() final {
    DrawTextureScaled(item->texture,
                      {(int)pos.x_ + DRAW_X, (int)pos.y_ + DRAW_Y,
                       (float)size.x, (float)size.y},
                      0, false, 0, WHITE);
    DRAW_HITBOXES();
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
