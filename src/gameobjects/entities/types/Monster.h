#ifndef DND_SRC_ENTITIES_MONSTER_H_
#define DND_SRC_ENTITIES_MONSTER_H_

#include "../../../ui/game/HealthBar.h"
#include "../../../util/GlobalVariables.h"
#include "../Entity.h"

struct Monster : public Entity {
  EntityStats stats;
  std::string name;

  bool dead = false;
  float pov;
  HealthBar health_bar;

  Monster() : Entity(), stats{}, pov(0), health_bar(0, 0) {}
  Monster(const Point& pos, EntityStats stats, const Point& size = {50, 50})
      : Entity(pos, size, ShapeType::RECT), pov(0), health_bar(size.x(), 15), stats(stats) {}
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
    if (health_bar.show) {
      DrawRectanglePro({pos.x(), pos.y(), size.x(), size.y()}, {0, 0}, pov, BLUE);
      health_bar.draw(pos.x(), pos.y(), stats);
      return;
    }
    DrawRectanglePro({pos.x(), pos.y(), size.x(), size.y()}, {0, 0}, pov, RED);

  }
  void update() final {
    if (stats.general.health <= 0) {
      dead = true;
    }
    health_bar.update();
  }
  void hit(DamageStats damage_stats) {
    std::cout<< "hey" << std::endl;
    health_bar.hit();
    stats.general.health -= damage_stats.damage;
  }
};
#endif  //DND_SRC_ENTITIES_MONSTER_H_
