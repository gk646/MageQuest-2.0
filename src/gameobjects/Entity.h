#ifndef MAGE_QUEST_SRC_ENTITY_H_
#define MAGE_QUEST_SRC_ENTITY_H_

#include "../pathfinding/PathFinder.h"

struct Entity {
  Point pos;
  PointT<int16_t> tile_pos;
  PointT<int16_t> size;
  int16_t pov;
  uint16_t spriteCounter = 0;
  ShapeType hitboxShape;
  bool isDead = false;
  Zone currentZone;
  bool isUpdated = true;
  bool isIlluminated = false;
  Entity(const Point& pos, const PointT<int16_t>& size, ShapeType hitboxShape,
         int16_t pov = 0, Zone zone = CURRENT_ZONE)
      : pos(pos), size(size), hitboxShape(hitboxShape), pov(pov), currentZone(zone) {}
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
  virtual void Update() = 0;
#define ENTITY_UPDATE()                                                  \
  tile_pos.x = static_cast<int16_t>(pos.x_ + size.x / 2.0F) / TILE_SIZE; \
  tile_pos.y = static_cast<int16_t>(pos.y_ + size.y / 2.0F) / TILE_SIZE; \
  isUpdated = currentZone == CURRENT_ZONE &&                             \
              PLAYER.tile_pos.dist(this->tile_pos) < UPDATE_DISTANCE;    \
  if (!isUpdated) return;

  virtual void Draw() = 0;
  [[nodiscard]] bool intersects(const Entity& o) const noexcept {
    if (pov == 0) {
      if (hitboxShape == ShapeType::RECT) {
        if (o.hitboxShape == ShapeType::RECT) {
          return (pos.x_ < o.pos.x_ + o.size.x && pos.x_ + size.x > o.pos.x_ &&
                  pos.y_ < o.pos.y_ + o.size.y && pos.y_ + size.y > o.pos.y_);
        } else if (o.hitboxShape == ShapeType::CIRCLE) {
          float other_radius_sq = o.size.x * o.size.x;
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
    } else {
      if (hitboxShape == ShapeType::RECT) {
        if (o.hitboxShape == ShapeType::RECT) {
          return SAT::rectanglesIntersect({pos.x_, pos.y_}, size.x, size.y, pov, o.pos,
                                          o.size.x, o.size.y, o.pov);
        }
      }
    }
  }
  void draw_hitbox() const {
    switch (hitboxShape) {
      case ShapeType::RECT:
        DrawRectangleOutlinePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x, size.y, {0, 0},
                                pov, 3, GREEN);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSectorLines(
            {pos.x_ + DRAW_X + size.x / 2.0F, pos.y_ + DRAW_Y + size.x / 2.0F}, size.x, 0,
            360, 50, GREEN);
        break;
    }
  };
  [[nodiscard]] inline bool tile_collision_left(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x / 2.0F - speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y / 2.0F) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_right(float speed) const noexcept {
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
           CheckTileCollision(entXRight, entYUp) && tile_collision_right(speed) ||
           tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x / 2.0F + speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y / 2.0F + speed) / TILE_SIZE;

    return !BoundCheckMap(entXRight, entYDown) ||
           CheckTileCollision(entXRight, entYDown) && tile_collision_right(speed) ||
           tile_collision_down(speed);
  }
  [[nodiscard]] inline bool tile_collision_upleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x / 2.0F - speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y / 2.0F - speed) / TILE_SIZE;

    return !BoundCheckMap(entXLeft, entYUp) ||
           CheckTileCollision(entXLeft, entYUp) && tile_collision_left(speed) ||
           tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x / 2.0F - speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y / 2.0F + speed) / TILE_SIZE;

    return !BoundCheckMap(entXLeft, entYDown) ||
           CheckTileCollision(entXLeft, entYDown) && tile_collision_left(speed) ||
           tile_collision_down(speed);
  }
  void CalculateMovement(const PointT<int16_t>& next, float speed) noexcept {
    bool canMoveRight = tile_pos.x < next.x && !tile_collision_right(speed);
    bool canMoveLeft = tile_pos.x > next.x && !tile_collision_left(speed);
    bool canMoveUp = tile_pos.y > next.y && !tile_collision_up(speed);
    bool canMoveDown = tile_pos.y < next.y && !tile_collision_down(speed);

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
#include "elements/ThreatManager.h"
#endif  //MAGE_QUEST_SRC_ENTITY_H_
