#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_

//A wrapper around "Item" with a hitbox and pickup logic
struct DroppedItem final : public WorldObject {
  Item* item;
  DroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {40, 40}, ShapeType::RECT, CURRENT_ZONE), item(item) {}

  inline void Draw() final {
    DrawTextureScaled(item->texture,
                      {std::floor(pos.x_ + DRAW_X), std::floor(pos.y_ + DRAW_Y),
                       (float)size.x, (float)size.y},
                      0, false, 0, WHITE);
    DRAW_HITBOXES();
  }
  inline void Update() final {
    ENTITY_UPDATE()
    if (this->Intersects(PLAYER)) {
      isDead = CharacterBag::AddItem(item);
    }
  }
  inline static bool DropItem(ItemSlot* slot) noexcept {
    if (!slot || !slot->item) return false;

    auto playerPos = PLAYER.GetMiddlePoint();
    playerPos.divide(48);
    for (auto& i : PathFinding::cross_movement) {
      if (!CheckTileCollision((int)playerPos.x() + i.x, (int)playerPos.y() + i.y)) {
        playerPos.scale(48);
        playerPos.x() += 44.0F * i.x;
        playerPos.y() += 44.0F * i.y;
        playerPos.x() -= 20;
        playerPos.y() -= 20;
        WORLD_OBJECTS.emplace_back(new DroppedItem(playerPos, slot->item));
        return true;
      }
    }
    return false;
    //TODO cant drop item notification
  }
  inline static void DropItemAimed(Item* item) noexcept {
    if (!item) return;

    auto playerPos = PLAYER.GetMiddlePoint();
    float angle = std::atan2(MOUSE_POS.y - playerPos.y_ - DRAW_Y,
                             MOUSE_POS.x - playerPos.x_ - DRAW_X);
    Vector2 mvmt = {std::cos(angle), std::sin(angle)};
    playerPos.x_ += 53 * mvmt.x;
    playerPos.y_ += 53 * mvmt.y;
    playerPos.x() -= 20;
    playerPos.y() -= 20;
    WORLD_OBJECTS.emplace_back(new DroppedItem(playerPos, item));
  }
};
//Same as "DroppedItem" but is animated in a Bézier curve once
struct AnimatedDroppedItem final : public WorldObject {
  Item* item;
  float arcPosition = 0;
  float startX, startY, endX{}, endY, controlX{}, controlY{};
  AnimatedDroppedItem(const Point& pos, Item* item)
      : WorldObject(pos, {40, 40}, ShapeType::RECT, CURRENT_ZONE),
        item(item),
        endY(pos.y_),
        startX(pos.x_),
        startY(pos.y_) {
    GetArcEndPoints();
  }

  void Update() final {
    ENTITY_UPDATE()
    if (arcPosition >= 1 && this->Intersects(PLAYER)) {
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

void ItemSlot::Update() noexcept {
  hitBox.height = baseWidth * UI_SCALE;
  hitBox.width = baseHeight * UI_SCALE;
  if (CheckCollisionPointRec(MOUSE_POS, hitBox)) {
    if (!DRAGGED_ITEM && item && IsKeyDown(KEY_X)) {
      DroppedItem::DropItem(this);
      item = nullptr;
      return;
    }

    if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        for (int i = 0; i < 10; i++) {
          if (NoSwitchDragRules(PLAYER_EQUIPPED[i])) {
            PLAYER_EQUIPPED[i].item = item;
            PLAYER_STATS.EquipItem(item->effects);
            item = nullptr;
            break;
          }
        }
      } else {
        DRAGGED_ITEM = item;
        DRAGGED_SLOT = this;
        item = nullptr;
      }
    } else if (DRAGGED_ITEM && !item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    } else if (DRAGGED_ITEM && item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
               BagSlotDropRuleSwitch()) {
      if (DRAGGED_SLOT->slotType != ItemType::EMPTY) {
        PLAYER_STATS.EquipItem(item->effects);
      }
      DRAGGED_SLOT->item = item;
      item = DRAGGED_ITEM;
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }
    toolTipHoverTicks = std::max(toolTipHoverTicks - 1, -1);
  } else {
    toolTipHoverTicks = 12;
  }
}
void ItemSlot::UpdateCharacterSlots() noexcept {
  hitBox.height = baseWidth * UI_SCALE;
  hitBox.width = baseHeight * UI_SCALE;
  if (CheckCollisionPointRec(MOUSE_POS, hitBox)) {
    if (!DRAGGED_ITEM && item && IsKeyDown(KEY_X)) {
      if(DroppedItem::DropItem(this)){
        auto savedItem = item;
        item = nullptr;
        PLAYER_STATS.UnEquipItem(savedItem->effects);
      }
      return;
    }

    if (!DRAGGED_ITEM && item && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (IsKeyDown(KEY_LEFT_SHIFT)) {
        for (int i = 0; i < (int)PLAYER_STATS.GetBagSlots(); i++) {
          if (!PLAYER_BAG[i].item) {
            PLAYER_BAG[i].item = item;
            item = nullptr;
            PLAYER_STATS.UnEquipItem( PLAYER_BAG[i].item->effects);
            break;
          }
        }
      } else {
        DRAGGED_ITEM = item;
        item = nullptr;
        DRAGGED_SLOT = this;
        PLAYER_STATS.UnEquipItem(DRAGGED_ITEM->effects);
      }
    } else if (DRAGGED_ITEM && !item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
               CharacterSlotDropRules()) {
      item = DRAGGED_ITEM;
      PlaySoundR(sound::equip);
      PLAYER_STATS.EquipItem(item->effects);
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    } else if (DRAGGED_ITEM && item && !IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
               CharacterSlotDropRules()) {
      PlaySoundR(sound::equip);
      PLAYER_STATS.EquipItem(DRAGGED_ITEM->effects);
      DRAGGED_SLOT->item = item;
      item = DRAGGED_ITEM;
      if (DRAGGED_SLOT->slotType == ItemType::EMPTY) {
        PLAYER_STATS.UnEquipItem(DRAGGED_SLOT->item->effects);
      }
      DRAGGED_SLOT = nullptr;
      DRAGGED_ITEM = nullptr;
    }
    toolTipHoverTicks = std::max(toolTipHoverTicks - 1, -1);
  } else {
    toolTipHoverTicks = 12;
  }
}
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_DROPPEDITEM_H_
