#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../ui/game/HealthBar.h"
struct Monster : public Entity {
  uint_fast32_t u_id = MONSTER_ID++;
  EntityStats stats;
  bool moving = false;
  int attack = 0;
  int attack_cd = 0;
  std::string name;
  MonsterResource* resource;
  StatusEffectHandler status_effects{stats};
  HealthBar health_bar{50, 10};
  Monster(const Point& pos, const EntityStats& stats, MonsterResource* resource,
          const Point& size = {50, 50}, ShapeType shape_type = ShapeType::RECT)
      : Entity(pos, size, shape_type),
        health_bar(size.x_, 10),
        stats(stats),
        resource(resource) {}
  Monster(const Monster& other)
      : Entity(other),
        stats(other.stats),
        name(other.name),
        health_bar(other.health_bar),
        resource(other.resource) {}
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
  void update() override {
    sprite_counter++;
    if (stats.health <= 0) {
      attack = -100;
    }
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
    health_bar.update();
    status_effects.update();
  };
  void draw() override = 0;
  void hit(Projectile& p) noexcept {
    if (p.from_player) {
      health_bar.hit();
      status_effects.add_effects(p.status_effects);
      stats.take_damage(p.damage_stats);
    }
    p.dead = p.from_player && attack != -100 && p.projectile_type == HitType::ONE_HIT;
  }
  bool move_to_player() noexcept {
    if (attack != 0) {
      return false;
    }
    PointI point;
    moving = false;
    if ((point = astar_pathfinding(tile_pos, *PLAYER_TILE)) > 0) [[likely]] {
      decideMovement(point,stats.get_speed());
      moving = true;
      return false;
    } else if (point == 0 && RANGE_01SMALL(RNG_RANDOM) > 30 &&
               !this->intersects(PLAYER)) {
      float speed = stats.get_speed();
      moving = true;
      if (pos.x_ < PLAYER_X) {
        pos.x_ += speed;
      } else if (pos.x_ > PLAYER_X) {
        pos.x_ -= speed;
      } else if (pos.y_ > PLAYER_Y) {
        pos.y_ -= speed;
      } else if (pos.y_ < PLAYER_Y) {
        pos.y_ += speed;
      }
    }
    return true;
  }
};

#include "monsters/SkeletonSpear.h"
#include "monsters/SkeletonWarrior.h"
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
