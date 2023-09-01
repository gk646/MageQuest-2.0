#ifndef DND_SRC_ENTITIES_PLAYER_H_
#define DND_SRC_ENTITIES_PLAYER_H_

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
  HotBar hotbar{10, 2};

  Player() : Entity(), stats{}, pov(0) {}
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
        class_(other.class_),
        hotbar(other.hotbar) {}
  Player& operator=(const Player& other) {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    stats = other.stats;
    name = other.name;
    pov = other.pov;
    class_ = other.class_;
    hotbar = other.hotbar;

    return *this;
  }
  void draw() final {
    DrawRectanglePro(CAMERA_X + size.x() / 2, CAMERA_Y + size.y() / 2, size.x(), size.y(), {0, 0},
                     pov, BLUE);
    hotbar.draw();
  }
  void update(std::vector<Projectile>* projectiles) {
    movement();
    abilities(projectiles);
    hotbar.update();
  }
  void movement() {
    if (IsKeyDown(KEY_W)) {
      pos.y() -= stats.general.speed;
    }
    if (IsKeyDown(KEY_S)) {
      pos.y() += stats.general.speed;
    }
    if (IsKeyDown(KEY_A)) {
      pos.x() -= stats.general.speed;
    }
    if (IsKeyDown(KEY_D)) {
      pos.x() += stats.general.speed;
    }
    PLAYER_X = pos.x();
    PLAYER_Y = pos.y();
  }
  void abilities(std::vector<Projectile>* projectiles) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      projectiles->push_back(PoisonBall(pos, GetMousePosition()));
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      projectiles->push_back(FireBall(pos, GetMousePosition()));
    }
  }
  void menu_control() {}
  void hit(DamageStats stats) {}
};
#endif  //DND_SRC_ENTITIES_PLAYER_H_
