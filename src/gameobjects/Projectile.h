#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_

struct Projectile : public Entity {
  bool from_player;
  float speed;
  int life_span;
  ProjectileType projectile_type = ProjectileType::ONE_HIT;
  DamageStats damage_stats;
  Point move_vector;
  //Texture2D  texture;
  std::vector<StatusEffect*> status_effects{};
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
            const Vector2& destination, int life_span, float speed,const DamageStats& damage_stats,
             ProjectileType type, const std::vector<StatusEffect*>& effects)
      : Entity(pos, size, shape_type),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type),
        status_effects(effects) {
    move_vector = get_move_vector(destination);
  }
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
             int life_span, float speed,const DamageStats& damage_stats, ProjectileType type,
             const std::vector<StatusEffect*>& effects, const Point& move_vector, float pov)
      : Entity(pos, size, shape_type,pov),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type),
        status_effects(effects),
        move_vector(move_vector) {}
  Projectile(const Projectile& p)
      : Entity(p),
        move_vector(p.move_vector),
        speed(p.speed),
        life_span(p.life_span),
        damage_stats(p.damage_stats),
        from_player(p.from_player),
        projectile_type(p.projectile_type) {
    for (const auto& effect : p.status_effects) {
      status_effects.push_back(effect->clone());
    }
  }
  Projectile& operator=(const Projectile& other) {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    // Clean up existing status_effects
    for (auto ptr : status_effects) {
      delete ptr;
    }
    status_effects.clear();

    // Deep copy other's status_effects
    for (const auto& effect : other.status_effects) {
      status_effects.push_back(effect->clone());
    }

    move_vector = other.move_vector;
    speed = other.speed;
    life_span = other.life_span;
    damage_stats = other.damage_stats;
    from_player = other.from_player;
    projectile_type = other.projectile_type;

    return *this;
  }
  ~Projectile() override {
    for (auto ptr : status_effects) {
      delete ptr;
    }
  }
  Point get_move_vector(const Vector2& mouse_pos) {
    float angle = std::atan2(mouse_pos.y - (pos.y() - PLAYER_Y + CAMERA_Y),
                             mouse_pos.x - (pos.x() - PLAYER_X + CAMERA_X));
    //pov =  angle * (180.0f / M_PI);
    return {std::cos(angle), std::sin(angle)};
  }
  void draw() override {
    switch (shape_type) {
      case ShapeType::RECT:
        DrawRectanglePro(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(),
                         size.y(), {0, 0}, pov, PURPLE);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSector(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(), 0,
                         360, 56, PURPLE);
        break;
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() override {
    pos.x() += move_vector.x() * speed;
    pos.y() += move_vector.y() * speed;
    life_span--;
    if (life_span <= 0) {
      dead = true;
    }
  }
};

#include "projectiles/FireBall_P.h"
#include "projectiles/PoisonBall.h"
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
