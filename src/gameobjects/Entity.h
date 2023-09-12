#ifndef MAGE_QUEST_SRC_ENTITY_H_
#define MAGE_QUEST_SRC_ENTITY_H_

#include "../pathfinding/PathFinder.h"

struct Entity {
  bool dead = false;
  float pov = 0;
  Point pos{};
  PointI tile_pos{};
  Point size{};
  ShapeType shape_type = ShapeType::RECT;
  Entity(const Point& pos, const Point& size, ShapeType shape_type, float pov = 0)
      : pos(pos), size(size), shape_type(shape_type), pov(pov) {}
  Entity(const Entity& o) noexcept
      : pos(o.pos), size(o.size), shape_type(o.shape_type), pov(o.pov) {}
  Entity& operator=(const Entity& other) {
    if (this == &other) {
      return *this;
    }

    pos = other.pos;
    size = other.size;
    shape_type = other.shape_type;
    pov = other.pov;

    return *this;
  }
  virtual ~Entity() = default;
  virtual void update() = 0;
  virtual void draw() = 0;
  [[nodiscard]] bool intersects(const Entity& o) const noexcept {
    if (dead || o.dead) {
      return false;
    }
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
          const float closestX = std::clamp(pos.x_, o.pos.x_, o.pos.x_ + o.size.x_);
          const float closestY = std::clamp(pos.y_, o.pos.y_, o.pos.y_ + o.size.y_);

          return ((closestX - pos.x_) * (closestX - pos.x_) +
                  (closestY - pos.y_) * (closestY - pos.y_)) <= radius_sq;
        } else if (o.shape_type == ShapeType::CIRCLE) {
          float other_radius_sq = o.size.x_ * o.size.x_;
          return ((pos.x_ - o.pos.x_) * (pos.x_ - o.pos.x_) +
                  (pos.y_ - o.pos.y_) * (pos.y_ - o.pos.y_)) <=
                 (radius_sq + other_radius_sq);
        }
      }
    } else {
      return false;
    }
  }
  void draw_hitbox() const {
    switch (shape_type) {
      case ShapeType::RECT:
        DrawRectangleOutlinePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_,
                                {0, 0}, pov, 5, RED);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSectorLines({pos.x_ + DRAW_X, pos.y_ + DRAW_Y}, size.x_, 0, 360, 50,
                              RED);
        break;
    }
  };
  [[nodiscard]] inline bool tile_collision_left(float speed) const noexcept {
    int entX = static_cast<int>(pos.x_ + size.x_ / 2 - speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  [[nodiscard]] inline bool tile_collision_right(float speed) const noexcept {
    int entX = static_cast<int>(pos.x_ + size.x_ / 2 + speed) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  [[nodiscard]] inline bool tile_collision_down(float speed) const noexcept {
    int entX = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2 + speed) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  [[nodiscard]] inline bool tile_collision_up(float speed) const noexcept {
    int entX = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;
    int entY = static_cast<int>(pos.y_ + size.y_ / 2 - speed) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  [[nodiscard]] inline bool tile_collision_upright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x_ / 2 + speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y_ / 2 - speed) / TILE_SIZE;

    return COLLISIONS[CURRENT_BACK_GROUND[entXRight][entYUp]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entXRight][entYUp]] == C_SOLID ||
           tile_collision_right(speed) || tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downright(float speed) const noexcept {
    int entXRight = static_cast<int>(pos.x_ + size.x_ / 2 + speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y_ / 2 + speed) / TILE_SIZE;

    return COLLISIONS[CURRENT_BACK_GROUND[entXRight][entYDown]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entXRight][entYDown]] == C_SOLID ||
           tile_collision_right(speed) || tile_collision_down(speed);
  }
  [[nodiscard]] inline bool tile_collision_upleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x_ / 2 - speed) / TILE_SIZE;
    int entYUp = static_cast<int>(pos.y_ + size.y_ / 2 - speed) / TILE_SIZE;

    return COLLISIONS[CURRENT_BACK_GROUND[entXLeft][entYUp]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entXLeft][entYUp]] == C_SOLID ||
           tile_collision_left(speed) || tile_collision_up(speed);
  }
  [[nodiscard]] inline bool tile_collision_downleft(float speed) const noexcept {
    int entXLeft = static_cast<int>(pos.x_ + size.x_ / 2 - speed) / TILE_SIZE;
    int entYDown = static_cast<int>(pos.y_ + size.y_ / 2 + speed) / TILE_SIZE;

    return COLLISIONS[CURRENT_BACK_GROUND[entXLeft][entYDown]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entXLeft][entYDown]] == C_SOLID ||
           tile_collision_left(speed) || tile_collision_down(speed);
  }
};
#include "WorldObject.h"

#endif  //MAGE_QUEST_SRC_ENTITY_H_
