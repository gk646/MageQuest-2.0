#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

#include "../../../gameplay/StatusEffectHandler.h"
#include "../../../gameplay/effects/Slow.h"
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
  StatusEffectComponent status_effects{stats};
  Player(Class a_class, Point pos, Point size = {25, 25}, ShapeType shape_type = ShapeType::RECT,
         float pov = 0, std::string name = "", EntityStats stats = {})
      : Entity(pos, size, shape_type),
        pov(pov),
        name(std::move(name)),
        stats(stats),
        class_(a_class) {}
  Player(const Player& other)
      : Entity(other), stats(other.stats), name(other.name), pov(other.pov), class_(other.class_) {}
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
    status_effects.draw();
  }
  void hit(Projectile& p) {
    if(!p.from_player){
      status_effects.add_effects(p.status_effects);
      stats.general.get_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
  void abilities(){
   if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
      status_effects.add_effects({new Slow(50,120)});
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
