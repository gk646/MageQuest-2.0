#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_

struct Projectile : public Entity {
  DamageStats damage_stats;
  bool from_player;
  bool does_damage = true;
  float speed;
  int life_span;
  int sprite_counter = 0;
  const Entity* Sender = nullptr;
  HitType projectile_type = HitType::ONE_HIT;
  Vector2 move_vector;
  ProjectileResources* resources = nullptr;
  std::vector<StatusEffect*> status_effects{};
  Sound* sound = nullptr;
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
             int life_span, float speed, const DamageStats& damage_stats, HitType type,
             const std::vector<StatusEffect*>& effects, const Vector2& move_vector,
             float pov, Sound* sound, ProjectileResources* res,
             const Entity* sender) noexcept
      : Entity(pos, size, shape_type, pov),
        life_span(life_span),
        speed(speed),
        damage_stats(damage_stats),
        from_player(from_player),
        projectile_type(type),
        status_effects(effects),
        move_vector(move_vector),
        resources(res),
        sound(sound),
        Sender(sender) {}
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
  inline Vector2 get_move_vector() noexcept {
    float angle = std::atan2(MOUSE_POS.y - pos.y_ - DRAW_Y - size.y_ / 2,
                             MOUSE_POS.x - pos.x_ - DRAW_X - size.x_ / 2);
    return {std::cos(angle), std::sin(angle)};
  }
  [[nodiscard]] inline bool active() const noexcept { return !dead && does_damage; }
  void update() noexcept override {
    sprite_counter++;
    pos.x_ += move_vector.x * speed;
    pos.y_ += move_vector.y * speed;
    tile_pos.x = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;
    life_span--;
    if (life_span <= 0) {
      dead = true;
    }
  }
};

#include "projectiles/FireBall.h"
#include "projectiles/Dummy.h"
#include "projectiles/PoisonBall.h"
#include "projectiles/BlastHammer.h"
#include "projectiles/EnergySphere.h"
#include "projectiles/FireSword.h"
#include "projectiles/FrostNova.h"
#include "projectiles/IceLance.h"
#include "projectiles/InfernoRay.h"
#include "projectiles/Lightning.h"
#include "projectiles/PowerSurge.h"
#include "projectiles/PyroBlast.h"
#include "projectiles/SolarFlare.h"
#include "projectiles/ThunderStrike.h"
#include "projectiles/ThunderSplash.h"
#include "projectiles/VoidEruption.h"
#include "projectiles/VoidField.h"
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
