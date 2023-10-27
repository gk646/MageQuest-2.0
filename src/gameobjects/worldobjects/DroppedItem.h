#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_

//A wrapper around "Item" with a hitbox and pickup logic
struct DroppedItem final : public WorldObject {
  Item* item;
  DroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {40, 40}, ShapeType::RECT, CURRENT_ZONE), item(item) {}

  void Update() final {
    ENTITY_UPDATE()
    if (this->intersects(PLAYER)) {
      isDead = CharacterBag::AddItem(item);
    }
  }
  void Draw() final {
    DrawTextureScaled(item->texture,
                      {std::floor(pos.x_ + DRAW_X), std::floor(pos.y_ + DRAW_Y),
                       (float)size.x, (float)size.y},
                      0, false, 0, WHITE);
    DRAW_HITBOXES();
  }
};
//Same as "DroppedItem" but is animated in a Bézier curve once
struct AnimatedDroppedItem final : public WorldObject {
  Item* item;
  float arcPosition = 0;
  float startX, startY, endX, endY, controlX, controlY;
  AnimatedDroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {40, 40}, ShapeType::RECT, CURRENT_ZONE),
        item(item),
        endY(pos.y_),
        startX(pos.y_),
        startY(pos.y_) {
    GetArcEndPoints();
  }

  void Update() final {
    ENTITY_UPDATE();
    if (arcPosition >= 1 && this->intersects(PLAYER)) {
      isDead = CharacterBag::AddItem(item);
    }
  }
  void Draw() final {
    if (arcPosition <= 1.0) {
      float t = arcPosition;
      float u = 1 - t;
      pos.x_ = u * u * startX + 2 * u * t * controlX + t * t * endX;
      pos.y_ = u * u * startY + 2 * u * t * controlY + t * t * endY;
      arcPosition += 0.01f;
    }
    DrawTextureScaled(item->texture,
                      {std::floor(pos.x_ + DRAW_X), std::floor(pos.y_ + DRAW_Y),
                       (float)size.x, (float)size.y},
                      0, false, 0, WHITE);
    DRAW_HITBOXES();
  }
  //Sets right values for the animation so items stays accessible
  inline void GetArcEndPoints() noexcept {
    endX = pos.x_ + 50;
    controlX = pos.x_ + 25;
    controlY = pos.y_ - 100;
  }
};
//Clears the previously owned slots and drops their items
void CharacterBag::RemoveSlots(int n) noexcept {
  int playerSlots = (int)PLAYER_STATS.GetBagSlots();
  for (int i = playerSlots; i < playerSlots + n; i++) {
    if (PLAYER_BAG[i].item) {
      WORLD_OBJECTS.emplace_back(
          new DroppedItem({PLAYER_X + 50, PLAYER_Y}, PLAYER_BAG[i].item));
    }
    PLAYER_BAG[i].item = nullptr;
  }
}

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
