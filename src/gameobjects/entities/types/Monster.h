#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../Entity.h"
#include "../../../system/GlobalVariables.h"
#include "../../../ui/game/HealthBar.h"
#include "Projectile.h"

struct Monster : public Entity {
  EntityStats stats;
  float pov;
  HealthBar health_bar;
  StatusEffectHandler status_effects{stats};
  std::string name;

  Monster(const Point& pos, EntityStats stats, const Point& size = {50, 50},
          ShapeType shape_type = ShapeType::RECT)
      : Entity(pos, size, shape_type), pov(0), health_bar(size.x(), 15), stats(stats) {}
  explicit Monster(const Point& pos, Point size = {50, 50}, ShapeType shape_type = ShapeType::RECT,
                   float pov = 0, std::string name = "", EntityStats stats = {})
      : Entity(pos, size, shape_type),
        pov(pov),
        name(std::move(name)),
        stats(stats),
        health_bar(15, size.y()) {}
  Monster(const Monster& other)
      : Entity(other),
        stats(other.stats),
        name(other.name),
        pov(other.pov),
        health_bar(other.health_bar) {}
  Monster& operator=(const Monster& other) {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    health_bar = other.health_bar;
    stats = other.stats;
    name = other.name;
    pov = other.pov;

    return *this;
  }
  void draw() final {
    if (shape_type == ShapeType::CIRCLE) {
      DrawCircleSector(pos.x()-PLAYER_X+CAMERA_X, pos.y()-PLAYER_Y+CAMERA_Y, size.x(), 0, 360, 50, RED);
    } else {
      DrawRectanglePro(pos.x()-PLAYER_X+CAMERA_X, pos.y()-PLAYER_Y+CAMERA_Y, size.x(),size.y(),{0, 0}, pov, RED);
    }
    if (health_bar.show) {
      health_bar.draw(pos.x()-PLAYER_X+CAMERA_X, pos.y()-PLAYER_Y+CAMERA_Y, stats);
    }
  }
  void update() final {
    if (stats.general.health <= 0) {
      dead = true;
    }
    health_bar.update();
  }
  void hit(Projectile& p) {
    health_bar.hit();
    stats.general.take_damage(p.damage_stats);

  }
};
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
