#ifndef DUNGEONM_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define DUNGEONM_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_

#include "../../../enums/Enums.h"
#include "../Entity.h"
#include "../attributes/Stats.h"

struct Projectile : public Entity {
  Point move_vector;
  bool dead = false;
  float speed = 1;

  int life_span_ticks = 0;
  int life_span;
  DamageStats damage_stats;

  Projectile(Point pos, Point size, ShapeType shape_type, Point origin, Vector2 destination,
             int life_span, float speed, DamageStats damage_stats = {})
      : Entity(pos, size, shape_type),
        dead{},
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats) {
    move_vector = get_move_vector(origin, destination);
  }
  Projectile(const Projectile& p)
      : Entity(p),
        move_vector(p.move_vector),
        dead(p.dead),
        speed(p.speed),
        life_span(p.life_span),
        life_span_ticks(p.life_span_ticks),
        damage_stats(p.damage_stats) {}
  Projectile& operator=(const Projectile& other) {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    move_vector = other.move_vector;
    dead = other.dead;
    speed = other.speed;
    life_span = other.life_span;
    life_span_ticks = other.life_span_ticks;
    damage_stats = other.damage_stats;

    return *this;
  }

  Point get_move_vector(Point origin, Vector2 mouse_pos) {
    float angle =
        std::atan2(mouse_pos.y - origin.y() - size.y(), mouse_pos.x - origin.x() - size.x());
    return {std::cos(angle), std::sin(angle)};
  }
  void draw() override {
    switch (shape_type) {
      case RECT:
        DrawRectanglePro({pos.x(), pos.y(), size.x(), size.y()}, {0, 0}, 0, PURPLE);
        break;
      case CIRCLE:
        DrawCircleSector({pos.x(), pos.y()},size.x(), 0,360,50,PURPLE);
    }
  }
  void update() override {
    pos.x() += move_vector.x() * speed;
    pos.y() += move_vector.y() * speed;
    life_span_ticks += 1;
    if (life_span_ticks >= life_span) {
      dead = true;
    }
  }
};
#endif  //DUNGEONM_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
