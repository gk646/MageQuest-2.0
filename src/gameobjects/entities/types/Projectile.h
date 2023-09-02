#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_

#include "../Entity.h"

struct Projectile final : public Entity {
  bool from_player;
  float speed = 1;
  int life_span_ticks = 0;
  int life_span;
  ProjectileType projectile_type = ProjectileType::ONE_HIT;
  DamageStats damage_stats;
  Point move_vector;
  std::vector<StatusEffect*> status_effects;
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type, Vector2 destination,
             int life_span, float speed, DamageStats damage_stats, ProjectileType type)
      : Entity(pos, size, shape_type),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type) {
    move_vector = get_move_vector(destination);
  }
  Projectile(const Projectile& p)
      : Entity(p),
        move_vector(p.move_vector),
        speed(p.speed),
        life_span(p.life_span),
        life_span_ticks(p.life_span_ticks),
        damage_stats(p.damage_stats), from_player(p.from_player) {}
  ~Projectile() final{
    for(auto ptr : status_effects){
      delete ptr;
    }
  }
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
  Point get_move_vector(Vector2 mouse_pos) {
    float angle = std::atan2(mouse_pos.y - (pos.y() - PLAYER_Y + CAMERA_Y),
                             mouse_pos.x - (pos.x() - PLAYER_X + CAMERA_X));
    return {std::cos(angle), std::sin(angle)};
  }
  void draw() override {
    switch (shape_type) {
      case ShapeType::RECT:
        DrawRectanglePro(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(),
                         size.y(), {0, 0}, 0, PURPLE);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSector(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(), 0,
                         360, 56, PURPLE);
        break;
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
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
