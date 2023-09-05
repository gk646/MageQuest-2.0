#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

struct Monster : public Entity {
  EntityStats stats;
  float pov = 0;
  HealthBar health_bar{50, 10};
  StatusEffectHandler status_effects{stats};
  MonsterResources* entity_resource = nullptr;
  std::string name;
  Monster(const Point& pos, const EntityStats& stats, const Point& size = {50, 50},
          ShapeType shape_type = ShapeType::RECT)
      : Entity(pos, size, shape_type), pov(0), health_bar(size.x(), 15), stats(stats) {}
  Monster(const Point& pos, const Point& size, ShapeType shape_type = ShapeType::RECT,
          float pov = 0, std::string name = "", const EntityStats& stats = {})
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
      DrawCircleSector(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(), 0,
                       360, 50, PURPLE);
    } else {
      DrawRectanglePro(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, size.x(),
                       size.y(), {0, 0}, pov, PURPLE);
    }
    if (health_bar.show) {
      health_bar.draw(pos.x() - PLAYER_X + CAMERA_X, pos.y() - PLAYER_Y + CAMERA_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() final {
    if (stats.general.health <= 0) {
      dead = true;
    }
    health_bar.update();
  }
  void hit(Projectile& p) {
    if (p.from_player) {
      health_bar.hit();
      status_effects.add_effects(p.status_effects);
      stats.general.take_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
};

#include <utility>

#include "monsters/SkeletonWarrior.h"
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
