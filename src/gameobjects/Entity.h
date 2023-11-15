#ifndef MAGE_QUEST_SRC_ENTITY_H_
#define MAGE_QUEST_SRC_ENTITY_H_

#include "../pathfinding/PathFinder.h"

//Base class of objects in the game
struct Entity {
  //Float
  Point pos;
  PointT<int16_t> tilePos;
  PointT<int16_t> size;
  //In degree
  int16_t pov;
  uint16_t spriteCounter = 0;
  ShapeType hitboxShape;
  bool isDead = false;
  int8_t actionState = 0;
  Zone currentZone;
  //Is in the same zone and closer than "UPDATE_RANGE"
  bool isUpdated = true;
  bool isIlluminated = false;
  Entity(const Point& pos, const PointT<int16_t>& size, ShapeType hitboxShape,
         int16_t pov = 0, bool isIlluminated = false, Zone zone = CURRENT_ZONE)
      : pos(pos),
        size(size),
        hitboxShape(hitboxShape),
        pov(pov),
        currentZone(zone),
        isIlluminated(isIlluminated) {}
  Entity(const Entity& o) noexcept
      : pos(o.pos),
        size(o.size),
        hitboxShape(o.hitboxShape),
        pov(o.pov),
        isDead(o.isDead),
        currentZone(o.currentZone),
        isIlluminated(o.isIlluminated) {}
  Entity& operator=(const Entity& other) noexcept {
    if (this == &other) {
      return *this;
    }

    isDead = other.isDead;
    pos = other.pos;
    size = other.size;
    hitboxShape = other.hitboxShape;
    pov = other.pov;
    isIlluminated = other.isIlluminated;
    currentZone = other.currentZone;

    return *this;
  }
  virtual ~Entity() = default;

#define ENTITY_UPDATE()                                                 \
  tilePos.x = static_cast<int16_t>(pos.x_ + size.x / 2.0F) / TILE_SIZE; \
  tilePos.y = static_cast<int16_t>(pos.y_ + size.y / 2.0F) / TILE_SIZE; \
  isUpdated = currentZone == CURRENT_ZONE &&                            \
              PLAYER.tilePos.dist(this->tilePos) < UPDATE_DISTANCE;     \
  if (!isUpdated) return;

  //Called on the draw thread
  virtual inline void Draw() = 0;
  //Called on the update thread
  virtual inline void Update() = 0;
  [[nodiscard]] inline bool Intersects(const Entity& o) const noexcept {
    if (hitboxShape == ShapeType::RECT) {
      if (pov != 0) {
        return SAT::RectanglesIntersect(GetMiddlePoint(), size.x, size.y, pov,
                                        o.GetMiddlePoint(), o.size.x, o.size.y, o.pov);
      }
      if (o.hitboxShape == ShapeType::RECT) {
        return (pos.x_ < o.pos.x_ + o.size.x && pos.x_ + size.x > o.pos.x_ &&
                pos.y_ < o.pos.y_ + o.size.y && pos.y_ + size.y > o.pos.y_);
      } else if (o.hitboxShape == ShapeType::CIRCLE) {
        const float other_radius_sq = o.size.x * o.size.x;
        const float closestX = std::clamp(o.pos.x_, pos.x_, pos.x_ + size.x);
        const float closestY = std::clamp(o.pos.y_, pos.y_, pos.y_ + size.y);

        return ((closestX - o.pos.x_) * (closestX - o.pos.x_) +
                (closestY - o.pos.y_) * (closestY - o.pos.y_)) <= other_radius_sq;
      }
    } else if (hitboxShape == ShapeType::CIRCLE) {
      float radius_sq = size.x * size.x;
      if (o.hitboxShape == ShapeType::RECT) {
        float newX = pos.x_ + size.x / 2.0F;
        float newY = pos.y_ + size.x / 2.0F;
        const float closestX = std::clamp(newX, o.pos.x_, o.pos.x_ + o.size.x);
        const float closestY = std::clamp(newY, o.pos.y_, o.pos.y_ + o.size.y);

        return ((closestX - newX) * (closestX - newX) +
                (closestY - newY) * (closestY - newY)) <= radius_sq;
      } else if (o.hitboxShape == ShapeType::CIRCLE) {
        float other_radius_sq = o.size.x * o.size.x;
        return ((pos.x_ - o.pos.x_) * (pos.x_ - o.pos.x_) +
                (pos.y_ - o.pos.y_) * (pos.y_ - o.pos.y_)) <=
               (radius_sq + other_radius_sq);
      }
    }
    return false;
  }
  //Returns the middle coordinates of itself as "Point"
  [[nodiscard]] inline Point GetMiddlePoint() const noexcept {
    return {pos.x_ + (float)size.x / 2.0F, pos.y_ + (float)size.y / 2.0F};
  }
  //Returns the top middle coordinates of itself as "Point"
  [[nodiscard]] inline Point GetUpperMiddle() const noexcept {
    return {pos.x_ + (float)size.x / 2.0F, pos.y_};
  }

 protected:
  inline void DrawHitbox() const {
    switch (hitboxShape) {
      case ShapeType::RECT:
        DrawRectOutlineMiddleRotation(
            {pos.x_ + DRAW_X, pos.y_ + DRAW_Y, static_cast<float>(size.x),
             static_cast<float>(size.y)},
            pov, GREEN);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSectorLines(
            {pos.x_ + DRAW_X + size.x / 2.0F, pos.y_ + DRAW_Y + size.x / 2.0F}, size.x, 0,
            360, 50, GREEN);
        break;
    }
  };
  [[nodiscard]] inline bool TileCollisionLeft(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x / 2.0F - speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y / 2.0F) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool TileCollisionRight(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x / 2.0F + speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y / 2.0F) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_down(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x / 2.0F) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y / 2.0F + speed) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_up(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x / 2.0F) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y / 2.0F - speed) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_upright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x / 2.0F + speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y / 2.0F - speed) / TILE_SIZE;

    return !BoundCheckMap(entXRight, entYUp) ||
           CheckTileCollision(entXRight, entYUp) && TileCollisionRight(speed) ||
           tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x / 2.0F + speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y / 2.0F + speed) / TILE_SIZE;

    return !BoundCheckMap(entXRight, entYDown) ||
           CheckTileCollision(entXRight, entYDown) && TileCollisionRight(speed) ||
           tile_collision_down(speed);
  }
  [[nodiscard]] inline bool tile_collision_upleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x / 2.0F - speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y / 2.0F - speed) / TILE_SIZE;

    return !BoundCheckMap(entXLeft, entYUp) ||
           CheckTileCollision(entXLeft, entYUp) && TileCollisionLeft(speed) ||
           tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x / 2.0F - speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y / 2.0F + speed) / TILE_SIZE;

    return !BoundCheckMap(entXLeft, entYDown) ||
           CheckTileCollision(entXLeft, entYDown) && TileCollisionLeft(speed) ||
           tile_collision_down(speed);
  }

  void CalculateMovement(const PointT<int16_t>& next, float speed) noexcept {
    bool canMoveRight = tilePos.x < next.x && !TileCollisionRight(speed);
    bool canMoveLeft = tilePos.x > next.x && !TileCollisionLeft(speed);
    bool canMoveUp = tilePos.y > next.y && !tile_collision_up(speed);
    bool canMoveDown = tilePos.y < next.y && !tile_collision_down(speed);

    if (RANGE_100(RNG_ENGINE) < 25) {
      if (canMoveRight && canMoveUp && !tile_collision_upright(speed)) {
        pos.x_ += speed;
        pos.y_ -= speed;
        return;
      } else if (canMoveRight && canMoveDown && !tile_collision_downright(speed)) {
        pos.x_ += speed;
        pos.y_ += speed;
        return;
      } else if (canMoveLeft && canMoveUp && !tile_collision_upleft(speed)) {
        pos.x_ -= speed;
        pos.y_ -= speed;
        return;
      } else if (canMoveLeft && canMoveDown && !tile_collision_downleft(speed)) {
        pos.x_ -= speed;
        pos.y_ += speed;
        return;
      }
    }

    if (canMoveRight) {
      pos.x_ += speed;
    } else if (canMoveLeft) {
      pos.x_ -= speed;
    } else if (canMoveUp) {
      pos.y_ -= speed;
    } else if (canMoveDown) {
      pos.y_ += speed;
    }
  }
};
#include "components/ThreatManager.h"

inline float EntityStats::TakeDamage(const DamageStats& stats, const Entity* ent) {
  if (RANGE_100_FLOAT(RNG_ENGINE) < effects[DODGE_CHANCE]) {
    DAMAGE_NUMBERS.emplace_back(FLT_MIN, DamageType::TRUE_DMG, ent->GetUpperMiddle());
    return 0.0F;
  }
  bool crit = false;
  float armour = GetArmour();
  float damage = stats.damage;
  if (RollCriticalHit(stats)) {
    crit = true;
    damage *= 1 + stats.critDamage;
  }

  damage *= 1 - effects[DAMAGE_RESISTANCE_P];
  if (stats.dmgType == DamageType::PHYSICAL) {
    damage *= 1 - armour / (level * 50.0F);
  } else if (stats.dmgType != DamageType::TRUE_DMG) {
    if (shield >= damage) {
      shield -= damage;
      return damage;
    } else {
      damage -= shield;
      shield = 0;
    }
  }
  if (health > 0) {
    lastHitType = stats.dmgType;
  }
  health -= damage;
  DAMAGE_NUMBERS.emplace_back(damage, stats.dmgType, ent->GetUpperMiddle(), crit);
  return damage;
}
#endif  //MAGE_QUEST_SRC_ENTITY_H_
