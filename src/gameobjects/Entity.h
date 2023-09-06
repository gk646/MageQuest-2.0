#ifndef MAGE_QUEST_SRC_ENTITY_H_
#define MAGE_QUEST_SRC_ENTITY_H_

struct Entity {
  bool dead = false;
  float pov = 0;
  Point pos;
  Point size;
  ShapeType shape_type;
  Entity() noexcept : pos{}, size{}, shape_type(ShapeType::RECT) {}
  Entity(const Point& pos, const Point& size, ShapeType shape_type, float pov = 0)
      : pos(pos), size(size), shape_type(shape_type), pov(pov) {}
  Entity(const Entity& o) noexcept : pos(o.pos), size(o.size), shape_type(o.shape_type), pov(o.pov) {}
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
  virtual ~Entity() {}
  virtual void update() = 0;
  virtual void draw() = 0;
  [[nodiscard]] bool intersects(const Entity& o) const noexcept {
    if (dead || o.dead) {
      return false;
    }
    if (pov == 0) {
      if (shape_type == ShapeType::RECT) {
        if (o.shape_type == ShapeType::RECT) {
          return (pos.x() < o.pos.x() + o.size.x() && pos.x() + size.x() > o.pos.x() &&
                  pos.y() < o.pos.y() + o.size.y() && pos.y() + size.y() > o.pos.y());
        } else if (o.shape_type == ShapeType::CIRCLE) {
          float other_radius_sq = o.size.x() * o.size.x();
          const float closestX = std::clamp(o.pos.x(), pos.x(), pos.x() + size.x());
          const float closestY = std::clamp(o.pos.y(), pos.y(), pos.y() + size.y());

          return ((closestX - o.pos.x()) * (closestX - o.pos.x()) +
                  (closestY - o.pos.y()) * (closestY - o.pos.y())) <= other_radius_sq;
        }
      } else if (shape_type == ShapeType::CIRCLE) {
        float radius_sq = size.x() * size.x();
        if (o.shape_type == ShapeType::RECT) {
          const float closestX = std::clamp(pos.x(), o.pos.x(), o.pos.x() + o.size.x());
          const float closestY = std::clamp(pos.y(), o.pos.y(), o.pos.y() + o.size.y());

          return ((closestX - pos.x()) * (closestX - pos.x()) +
                  (closestY - pos.y()) * (closestY - pos.y())) <= radius_sq;
        } else if (o.shape_type == ShapeType::CIRCLE) {
          float other_radius_sq = o.size.x() * o.size.x();
          return ((pos.x() - o.pos.x()) * (pos.x() - o.pos.x()) +
                  (pos.y() - o.pos.y()) * (pos.y() - o.pos.y())) <= (radius_sq + other_radius_sq);
        }
      }
    } else {
      return false;
    }
  }
  void draw_hitbox() {
    switch (shape_type) {
      case ShapeType::RECT:
        DrawRectangleOutlinePro(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(),
                         size.y(), {0, 0}, pov,5, RED);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSectorLines({pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y},size.x(), 0,
                         360, 50, RED);
        break;
    }
  };
  bool tile_collision_left(float speed) {
    int entX = (pos.x() + size.x() / 2 - speed) / TILE_SIZE;
    int entY = (pos.y() + size.y() / 2) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  bool tile_collision_right(float speed) {
    int entX = (pos.x() + size.x() / 2 + speed) / TILE_SIZE;
    int entY = (pos.y() + size.y() / 2) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  bool tile_collision_down(float speed) {
    int entX = (pos.x() + size.x() / 2) / TILE_SIZE;
    int entY = (pos.y() + size.y() / 2 + speed) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
  bool tile_collision_up(float speed) {
    int entX = (pos.x() + size.x() / 2) / TILE_SIZE;
    int entY = (pos.y() + size.y() / 2 - speed) / TILE_SIZE;
    return COLLISIONS[CURRENT_BACK_GROUND[entX][entY]] == C_SOLID ||
           COLLISIONS[CURRENT_MIDDLE_GROUND[entX][entY]] == C_SOLID;
  }
};
#endif  //MAGE_QUEST_SRC_ENTITY_H_
