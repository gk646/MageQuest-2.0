#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

#include "../../../gameplay/StatusEffectHandler.h"
#include "../../../gameplay/effects/Slow.h"
#include "../../../system/Enums.h"
#include "../../../system/GlobalVariables.h"
#include "../../../ui/player/HotBar.h"
#include "../Entity.h"
#include "../types/Projectile.h"
#include "projectiles/FireBall.h"
#include "projectiles/PoisonBall.h"

struct Player : public Entity {
  EntityStats stats;
  std::string name;
  float pov;
  HotBar hot_bar{5,1};
  StatusEffectHandler status_effects{stats};
  explicit Player(const Point& pos,const Point& size = {25, 25})
      : Entity(pos, size, ShapeType::RECT),
        pov(0),
        name("New Player"),
        stats({}){}
  Player(const Player& other)
      : Entity(other), stats(other.stats), name(other.name), pov(other.pov) {}
  Player& operator=(const Player& other) {
    if (this == &other) {
      return *this;
    }
    Entity::operator=(other);

    stats = other.stats;
    name = other.name;
    pov = other.pov;
    return *this;
  }
  void draw() final {
    DrawRectanglePro(CAMERA_X - size.x() / 2, CAMERA_Y - size.y() / 2, size.x(), size.y(), {0, 0},
                     pov, BLUE);
  }
  void hit(Projectile& p) {
    if (!p.from_player) {
      status_effects.add_effects(p.status_effects);
      stats.general.take_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
  void abilities() {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      status_effects.add_effects({new Slow(50, 120)});
    }
  }
  void update() final {
    status_effects.update();
    movement();
    abilities();
    PLAYER_TILE_X = (pos.x() + size.x() / 2) / TILE_SIZE;
    PLAYER_TILE_Y = (pos.y() + size.y() / 2) / TILE_SIZE;
  }
  void movement() {
    if (IsKeyDown(KEY_W) && !tile_collision_up(stats.general.speed)) {
      pos.y() -= stats.general.speed;
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(stats.general.speed)) {
      pos.y() += stats.general.speed;
    }
    if (IsKeyDown(KEY_A) && !tile_collision_left(stats.general.speed)) {
      pos.x() -= stats.general.speed;
    }
    if (IsKeyDown(KEY_D) && !tile_collision_right(stats.general.speed)) {
      pos.x() += stats.general.speed;
    }

    PLAYER_X = pos.x();
    PLAYER_Y = pos.y();
  }
};
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
