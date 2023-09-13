#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_

struct Projectile : public Entity {
  ProjectileType type;
  uint_fast32_t u_id;
  DamageStats damage_stats;
  bool from_player;
  float speed;
  int life_span;
  int sprite_counter = 0;
  HitType projectile_type = HitType::ONE_HIT;
  Point move_vector;
  ProjectileResources* resources = nullptr;
  std::vector<StatusEffect*> status_effects{};
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
             const Vector2& destination, int life_span, float speed,
             const DamageStats& damage_stats, HitType type,
             const std::vector<StatusEffect*>& effects, ProjectileType p_type) noexcept
      : Entity(pos, size, shape_type),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type),
        status_effects(effects) , type(p_type),u_id(PROJECTILE_ID++){
    move_vector = get_move_vector(destination);
  }
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
             int life_span, float speed, const DamageStats& damage_stats, HitType type, const std::vector<StatusEffect*>& effects,
             const Point& move_vector, float pov, ProjectileType p_type) noexcept
      : Entity(pos, size, shape_type, pov),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type),
        status_effects(effects),
        move_vector(move_vector) ,type(p_type),u_id(PROJECTILE_ID++){}
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
             int life_span, float speed, const DamageStats& damage_stats, HitType type, const std::vector<StatusEffect*>& effects,
             const Point& move_vector, float pov, ProjectileType p_type,uint_fast32_t u_id) noexcept
      : Entity(pos, size, shape_type, pov),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type),
        status_effects(effects),
        move_vector(move_vector) ,type(p_type),u_id(u_id){}
  Projectile(const Projectile& p) noexcept
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
  Projectile& operator=(const Projectile& other) noexcept {
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
  ~Projectile() noexcept override {
    for (auto ptr : status_effects) {
      delete ptr;
    }
  }
  Point get_move_vector(const Vector2& mouse_pos) noexcept {
    float angle =
        std::atan2(mouse_pos.y - (pos.y_ + DRAW_Y), mouse_pos.x - (pos.x_ + DRAW_X));
    //pov =  angle * (180.0f / M_PI);
    return {std::cos(angle), std::sin(angle)};
  }
  void draw() override  {
    switch (shape_type) {
      case ShapeType::RECT:
        DrawRectanglePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_, {0, 0}, pov,
                         PURPLE);
        break;
      case ShapeType::CIRCLE:
        DrawCircleSector(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, 0, 360, 56, PURPLE);
        break;
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() noexcept final {
    sprite_counter++;
    pos.x_ += move_vector.x() * speed;
    pos.y_ += move_vector.y() * speed;
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
    life_span--;
    if (life_span <= 0) {
      dead = true;
    }
  }
};

#include "projectiles/FireBall_P.h"
#include "projectiles/PoisonBall.h"
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
