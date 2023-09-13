#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../ui/game/HealthBar.h"
struct Monster : public Entity {
  uint_fast32_t u_id = MONSTER_ID++;
  EntityStats stats;
  bool moving = false;
  int attack = 0;
  int sprite_counter = 0;
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
        sprite_counter(other.sprite_counter),
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
  void update() override = 0;
  void draw() override {
    if (shape_type == ShapeType::CIRCLE) {
      DrawCircleSector(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, 0, 360, 50, PURPLE);
    } else {
      DrawRectanglePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_, {0, 0},
                       pov, PURPLE);
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void hit(Projectile& p)noexcept {
    if (p.from_player && MP_TYPE!= MultiplayerType::CLIENT) {
      health_bar.hit();
      status_effects.add_effects(p.status_effects);
      stats.take_damage(p.damage_stats);
    }
    p.dead = p.from_player&& attack != -100 && p.projectile_type == HitType::ONE_HIT;
  }
  inline void monster_update() noexcept {
    sprite_counter++;
    if (stats.health <= 0) {
      attack = -100;
    }
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
    health_bar.update();
    status_effects.update();
  }
  bool move_to_player() noexcept {
    if(attack != 0){
      return false;
    }
    PointI point;
    moving = false;
    if ((point = astar_pathfinding(tile_pos, *PLAYER_TILE)) > 0) [[likely]] {
      decideMovement(point.x, point.y);
      moving = true;
      return false;
    } else if (point == 0 &&RANGE_01SMALL(RNG_RANDOM) > 30 && !this->intersects(PLAYER)) {
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
  void decideMovement(int nextX, int nextY)noexcept {
    float speed = stats.get_speed();
    bool canMoveRight = tile_pos.x < nextX && !tile_collision_right(speed);
    bool canMoveLeft = tile_pos.x > nextX && !tile_collision_left(speed);
    bool canMoveUp = tile_pos.y > nextY && !tile_collision_up(speed);
    bool canMoveDown = tile_pos.y < nextY && !tile_collision_down(speed);

    if (RANGE_100(RNG_ENGINE) < 25) {
      if (canMoveRight && canMoveUp && !tile_collision_upright(speed)) {
        pos.x_ += speed;
        pos.y_ -= speed;
        return;
      } else if (canMoveRight && canMoveDown && !tile_collision_downright(speed)) {
        pos.x_ += speed;
        pos.y_ += speed;
        return;
      } else if (canMoveLeft && canMoveUp && !tile_collision_upleft(speed)) {
        pos.x_ -= speed;
        pos.y_ -= speed;
        return;
      } else if (canMoveLeft && canMoveDown && !tile_collision_downleft(speed)) {
        pos.x_ -= speed;
        pos.y_ += speed;
        return;
      }
    }

    if (canMoveRight) {
      pos.x_ += speed;
    } else if (canMoveLeft) {
      pos.x_ -= speed;
    } else if (canMoveUp) {
      pos.y_ -= speed;
    } else if (canMoveDown) {
      pos.y_ += speed;
    }
  }
};

#include "monsters/SkeletonSpear.h"
#include "monsters/SkeletonWarrior.h"
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
