#ifndef DND_SRC_ENTITIES_PLAYER_H_
#define DND_SRC_ENTITIES_PLAYER_H_

#include "../../../enums/Enums.h"
#include "../../../ui/player/HotBar.h"
#include "../Entity.h"
#include "../types/Projectile.h"

#include "../../../util/GlobalVariables.h"

struct Player : public Entity {
  EntityStats stats;
  std::string name;

  float pov;

  Class class_;
  Hotbar hotbar{10, 2};

  Player() : Entity(), stats{}, pov(0) {}
  Player(Class a_class, Point pos, Point size = {50, 50}, ShapeType shape_type = ShapeType::RECT,
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
    DrawRectanglePro({pos.x(), pos.y(), size.x(), size.y()}, {0, 0}, pov, BLUE);
    hotbar.draw(UI_SCALE, SCREEN_WIDTH, SCREEN_HEIGHT);
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
  }
  void abilities(std::vector<Projectile>* projectiles) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      projectiles->push_back(Projectile(pos + size / 2 + Point(-12.5, -12.5), {25, 25},
                                        ShapeType::RECT, pos, GetMousePosition(), 240, 5));
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      projectiles->push_back(Projectile(pos + size / 2, {40, 40},
                                        ShapeType::CIRCLE, pos, GetMousePosition(), 240, 3));
    }
  }
  void menu_control() {}
  void hit(DamageStats stats) {}
};
#endif  //DND_SRC_ENTITIES_PLAYER_H_
