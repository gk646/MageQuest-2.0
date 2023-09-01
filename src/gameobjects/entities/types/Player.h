#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

#include "../../../system/GlobalVariables.h"
#include "../../../ui/player/HotBar.h"
#include "../../../util/Enums.h"
#include "../Entity.h"
#include "../types/Projectile.h"
#include "projectiles/FireBall.h"
#include "projectiles/PoisonBall.h"

struct Player : public Entity {
  EntityStats stats;
  std::string name;

  float pov;
  Class class_;

  Player() : Entity(), stats{}, pov(0), class_(Class::DRUID) {}
  Player(Class a_class, Point pos, Point size = {25, 25}, ShapeType shape_type = ShapeType::RECT,
         float pov = 0, std::string name = "", EntityStats stats = {})
      : Entity(pos, size, shape_type),
        pov(pov),
        name(std::move(name)),
        stats(stats),
        class_(a_class) {}
  Player(const Player& other)
      : Entity(other),
        stats(other.stats),
        name(other.name),
        pov(other.pov),
        class_(other.class_){}
  Player& operator=(const Player& other) {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    stats = other.stats;
    name = other.name;
    pov = other.pov;
    class_ = other.class_;

    return *this;
  }
  void draw() final {
    DrawRectanglePro(CAMERA_X - size.x() / 2, CAMERA_Y - size.y() / 2, size.x(), size.y(), {0, 0},
                     pov, BLUE);
  }
  void update() final{
    movement();
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
  void hit(DamageStats damage_stats) {}
};
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
