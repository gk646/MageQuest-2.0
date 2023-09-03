#ifndef MAGE_QUEST_SRC_ENTITY_H_
#define MAGE_QUEST_SRC_ENTITY_H_

using namespace cxstructs;

struct Entity {
  ShapeType shape_type;

  Point pos;
  Point size;
  bool dead = false;

  Entity() : pos{}, size{}, shape_type(ShapeType::RECT) {}
  Entity(const Point& pos, const Point& size, ShapeType shape_type)
      : pos(pos), size(size), shape_type(shape_type) {}
  Entity(const Entity& o) : pos(o.pos), size(o.size), shape_type(o.shape_type) {}
  Entity& operator=(const Entity& other) {
    if (this == &other) {
      return *this;
    }

    pos = other.pos;
    size = other.size;
    shape_type = other.shape_type;

    return *this;
  }
  virtual ~Entity() {}
  virtual void update() = 0;
  virtual void draw() = 0;
  [[nodiscard]] bool intersects(const Entity& o) const {
    if (dead || o.dead) {
      return false;
    }
    if (shape_type == ShapeType::RECT) {
      if (o.shape_type == ShapeType::RECT) {
        return (pos.x() < o.pos.x() + o.size.x() && pos.x() + size.x() > o.pos.x() &&
                pos.y() < o.pos.y() + o.size.y() && pos.y() + size.y() > o.pos.y());
      } else if (o.shape_type == ShapeType::CIRCLE) {
        const float closestX = std::clamp(o.pos.x(), pos.x(), pos.x() + size.x());
        const float closestY = std::clamp(o.pos.y(), pos.y(), pos.y() + size.y());

        const float dx = closestX - o.pos.x();
        const float dy = closestY - o.pos.y();

        return (dx * dx + dy * dy) <= (o.size.x() * o.size.x());
      }
    } else if (shape_type == ShapeType::CIRCLE) {
      if (o.shape_type == ShapeType::RECT) {
        const float closestX = std::clamp(pos.x(), o.pos.x(), o.pos.x() + o.size.x());
        const float closestY = std::clamp(pos.y(), o.pos.y(), o.pos.y() + o.size.y());

        const float dx = closestX - pos.x();
        const float dy = closestY - pos.y();

        return (dx * dx + dy * dy) <= (size.x() * size.x());
      } else if (o.shape_type == ShapeType::CIRCLE) {
        return (pos.x() - o.pos.x()) * (pos.x() - o.pos.x()) +
                   (pos.y() - o.pos.y()) * (pos.y() - o.pos.y()) <=
               (size.x() + o.size.x()) * (size.x() + o.size.x());
      }
    }
  }
  void draw_hitbox(){
      if(shape_type== ShapeType::RECT){
      DrawRectangleLinesEx({pos.x(), pos.y(), size.x(), size.y()}, 3,RED);
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
