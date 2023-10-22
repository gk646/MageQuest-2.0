#ifndef MAGE_QUEST_SRC_ENTITY_H_
#define MAGE_QUEST_SRC_ENTITY_H_

#include "../pathfinding/PathFinder.h"

struct Entity {
  PointI tile_pos;
  Point pos;
  Point size;
  float pov;
  uint16_t spriteCounter = 0;
  ShapeType shape_type;
  bool dead = false;
  Zone zone;
  bool active = true;
  bool illuminated = false;
  Entity(const Point& pos, const Point& size, ShapeType shape_type, float pov = 0,
         Zone zone = CURRENT_ZONE)
      : pos(pos), size(size), shape_type(shape_type), pov(pov), zone(zone) {}
  Entity(const Entity& o) noexcept
      : pos(o.pos),
        size(o.size),
        shape_type(o.shape_type),
        pov(o.pov),
        dead(o.dead),
        zone(o.zone),
        illuminated(o.illuminated) {}
  Entity& operator=(const Entity& other) noexcept {
    if (this == &other) {
      return *this;
    }

    dead = other.dead;
    pos = other.pos;
    size = other.size;
    shape_type = other.shape_type;
    pov = other.pov;
    illuminated = other.illuminated;
    zone = other.zone;

    return *this;
  }
  virtual ~Entity() = default;
  virtual void Update() = 0;
#define ENTITY_UPDATE()                                                               \
  tile_pos.x = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;                    \
  tile_pos.y = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;                    \
  active =                                                                            \
      zone == CURRENT_ZONE && PLAYER.tile_pos.dist(this->tile_pos) < UPDATE_DISTANCE; \
  if (!active) return;

  virtual void Draw() = 0;
  [[nodiscard]] bool intersects(const Entity& o) const noexcept {
    if (pov == 0) {
      if (shape_type == ShapeType::RECT) {
        if (o.shape_type == ShapeType::RECT) {
          return (pos.x_ < o.pos.x_ + o.size.x_ && pos.x_ + size.x_ > o.pos.x_ &&
                  pos.y_ < o.pos.y_ + o.size.y_ && pos.y_ + size.y_ > o.pos.y_);
        } else if (o.shape_type == ShapeType::CIRCLE) {
          float other_radius_sq = o.size.x_ * o.size.x_;
          const float closestX = std::clamp(o.pos.x_, pos.x_, pos.x_ + size.x_);
          const float closestY = std::clamp(o.pos.y_, pos.y_, pos.y_ + size.y_);

          return ((closestX - o.pos.x_) * (closestX - o.pos.x_) +
                  (closestY - o.pos.y_) * (closestY - o.pos.y_)) <= other_radius_sq;
        }
      } else if (shape_type == ShapeType::CIRCLE) {
        float radius_sq = size.x_ * size.x_;
        if (o.shape_type == ShapeType::RECT) {
          float newX = pos.x_ + size.x_ / 2;
          float newY = pos.y_ + size.x_ / 2;
          const float closestX = std::clamp(newX, o.pos.x_, o.pos.x_ + o.size.x_);
          const float closestY = std::clamp(newY, o.pos.y_, o.pos.y_ + o.size.y_);

          return ((closestX - newX) * (closestX - newX) +
                  (closestY - newY) * (closestY - newY)) <= radius_sq;
        } else if (o.shape_type == ShapeType::CIRCLE) {
          float other_radius_sq = o.size.x_ * o.size.x_;
          return ((pos.x_ - o.pos.x_) * (pos.x_ - o.pos.x_) +
                  (pos.y_ - o.pos.y_) * (pos.y_ - o.pos.y_)) <=
                 (radius_sq + other_radius_sq);
        }
      }
    } else {
      if (shape_type == ShapeType::RECT) {
        if (o.shape_type == ShapeType::RECT) {
          return SAT::rectanglesIntersect({pos.x_, pos.y_}, size.x_, size.y_, pov, o.pos,
                                          o.size.x_, o.size.y_, o.pov);
        }
      }
    }
  }
  void draw_hitbox() const {
    switch (shape_type) {
      case ShapeType::RECT:
        DrawRectangleOutlinePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_,
                                {0, 0}, pov, 3, GREEN);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSectorLines(
            {pos.x_ + DRAW_X + size.x_ / 2, pos.y_ + DRAW_Y + size.x_ / 2}, size.x_, 0,
            360, 50, GREEN);
        break;
    }
  };
  [[nodiscard]] inline bool tile_collision_left(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x_ / 2 - speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_right(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x_ / 2 + speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_down(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2 + speed) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_up(float speed) const noexcept {
#ifdef NO_CLIP
    return false;
#endif
    int entX = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2 - speed) / TILE_SIZE;
    return !BoundCheckMap(entX, entY) || CheckTileCollision(entX, entY);
  }
  [[nodiscard]] inline bool tile_collision_upright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x_ / 2 + speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y_ / 2 - speed) / TILE_SIZE;

    return !BoundCheckMap(entXRight, entYUp) ||
           CheckTileCollision(entXRight, entYUp) && tile_collision_right(speed) ||
           tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x_ / 2 + speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y_ / 2 + speed) / TILE_SIZE;

    return !BoundCheckMap(entXRight, entYDown) ||
           CheckTileCollision(entXRight, entYDown) && tile_collision_right(speed) ||
           tile_collision_down(speed);
  }
  [[nodiscard]] inline bool tile_collision_upleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x_ / 2 - speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y_ / 2 - speed) / TILE_SIZE;

    return !BoundCheckMap(entXLeft, entYUp) ||
           CheckTileCollision(entXLeft, entYUp) && tile_collision_left(speed) ||
           tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x_ / 2 - speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y_ / 2 + speed) / TILE_SIZE;

    return !BoundCheckMap(entXLeft, entYDown) ||
           CheckTileCollision(entXLeft, entYDown) && tile_collision_left(speed) ||
           tile_collision_down(speed);
  }
  void decideMovement(const PointI& next, float speed) noexcept {
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
