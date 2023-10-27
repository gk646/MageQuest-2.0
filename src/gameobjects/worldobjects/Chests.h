#ifndef MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_CHESTS_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_CHESTS_H_

struct BasicChest final : public InteractableObject {
  explicit BasicChest(const PointI& pos, bool isUsed, Zone zone) noexcept
      : InteractableObject(textures::container::basicChest,
                           {(float)pos.x * 48, (float)pos.y * 48},
                           PointT<int16_t>{48, 46}, ShapeType::RECT, isUsed, zone) {}
  void Interact() final {
    WORLD_OBJECTS.emplace_back(new AnimatedDroppedItem(pos,ItemDropHandler::GetRandomScaledItemRarityGoal(ItemRarity::NORMAL,PLAYER_STATS.level)));
  }
};

struct NormalChest final : public InteractableObject {
  NormalChest(const PointI& pos, bool isUsed, Zone zone) noexcept
      : InteractableObject(textures::container::basicChest,
                           {(float)pos.x * 48, (float)pos.y * 48},
                           PointT<int16_t>{48, 46}, ShapeType::RECT, isUsed, zone) {}
  void Interact() final {}
};

struct EpicChest final : public InteractableObject {
  EpicChest(const PointI& pos, bool isUsed, Zone zone) noexcept
      : InteractableObject(textures::container::basicChest,
                           {(float)pos.x * 48, (float)pos.y * 48},
                           PointT<int16_t>{48, 46}, ShapeType::RECT, isUsed, zone) {}
  void Interact() final {}
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_WORLDOBJECTS_CHESTS_H_
