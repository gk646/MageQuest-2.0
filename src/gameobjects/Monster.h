#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../ui/game/HealthBar.h"
struct Monster : public Entity {
  StatusEffectHandler status_effects{stats};
  EntityStats stats;
  uint16_t u_id = MONSTER_ID++;
  int16_t attack = 0;
  int16_t attack_cd = 0;
  bool moving = false;
  bool prev_moving = false;
  bool flip = false;
  Entity* target = nullptr;
  MonsterResource* resource;
  std::string name;
  HealthBar health_bar{50, 10};
  MonsterType type;
  Monster(const Point& pos, const EntityStats& stats, MonsterResource* resource,
          MonsterType type, const Point& size = {50, 50},
          ShapeType shape_type = ShapeType::RECT)
      : Entity(pos, size, shape_type),
        health_bar(size.x_, 10),
        stats(stats),
        resource(resource),
        type(type) {
    if (MP_TYPE == MultiplayerType::SERVER) {
      Server::SendMsgToAllUsers(
          UDP_MONSTER_SPAWN,
          new UDP_MonsterSpawn(u_id, type, stats.level, static_cast<uint16_t>(pos.x_),
                               static_cast<uint16_t>(pos.y_)),
          sizeof(UDP_MonsterSpawn));
    }
  }
  Monster(const Monster& other)
      : Entity(other),
        stats(other.stats),
        name(other.name),
        health_bar(other.health_bar),
        resource(other.resource),
        type(other.type) {}
  Monster& operator=(const Monster& other) {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    type = other.type;
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
    if (p.from_player && p.active()) {
      health_bar.hit();
      status_effects.add_effects(p.status_effects);
      stats.take_damage(p.damage_stats);
      p.dead = attack != -100 && p.projectile_type == HitType::ONE_HIT;
    }
  }
  bool move_to_player() noexcept {
    if (attack != 0 || MP_TYPE == MultiplayerType::CLIENT) {
      return false;
    }
    flip = pos.x_ + size.x_ / 2 > MIRROR_POINT;
    PointI point;
    moving = false;
    if ((point = astar_pathfinding(tile_pos, *PLAYER_TILE)) > 0) [[likely]] {
      decideMovement(point, stats.get_speed());
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
  inline void update_state(UDP_MonsterUpdate* data) noexcept {
    if (stats.health != data->new_health) {
      stats.health = data->new_health;
      health_bar.hit();
    }

    if ((attack == 0 && sprite_counter > 100) || data->action_state == -100) {
      sprite_counter = 0;
      attack = data->action_state;
    }

    flip = (data->x < pos.x_);
    if (pos.x_ == data->x && pos.y_ == data->y) {
      if (prev_moving) {
        prev_moving = false;
        return;
      }
      moving = false;
    } else {
      moving = true;
      prev_moving = true;
    }
    pos.x_ = data->x;
    pos.y_ = data->y;
  }
};
#include "monsters/SkeletonSpear.h"
#include "monsters/SkeletonWarrior.h"
#include "monsters/Wolf.h"

void Server::SynchronizeMonsters(const SteamNetworkingIdentity& identity) noexcept {
  for (auto m : MONSTERS) {
    SendMsgToUser(identity, UDP_MONSTER_SPAWN,
                  new UDP_MonsterSpawn(m->u_id, m->type, m->stats.level,
                                       static_cast<uint16_t>(m->pos.x_),
                                       static_cast<uint16_t>(m->pos.y_)),
                  sizeof(UDP_MonsterSpawn));
  }
}
void Server::BroadCastMonsterUpdates() noexcept {
  for (auto m : MONSTERS) {
    if (m->moving || m->health_bar.delay > 0 || m->attack != 0) {
      Server::SendMsgToAllUsers(
          UDP_MONSTER_UPDATE,
          new UDP_MonsterUpdate(m->u_id, static_cast<uint16_t>(m->pos.x_),
                                static_cast<uint16_t>(m->pos.y_), m->stats.health,
                                static_cast<int8_t>(m->attack)),
          sizeof(UDP_MonsterUpdate));
    }
  }
}
void Multiplayer::HandleMonsterSpawn(UDP_MonsterSpawn* data) noexcept {
  switch (data->type) {
    case MonsterType::SKEL_WAR: {
      auto m = new SkeletonWarrior({(float)data->x, (float)data->y}, data->level);
      m->u_id = data->monster_id;
      MONSTERS.push_back(m);
    } break;
    case MonsterType::ANY:
      break;
    case MonsterType::SKEL_SPEAR: {
      auto m = new SkeletonSpear({(float)data->x, (float)data->y}, data->level);
      m->u_id = data->monster_id;
      MONSTERS.push_back(m);
    } break;
    case MonsterType::WOLF:
      break;
    case MonsterType::BOSS_DEATH_BRINGER:
      break;
    case MonsterType::BOSS_KNIGHT:
      break;
    case MonsterType::BOSS_SLIME:
      break;
    case MonsterType::GOBLIN:
      break;
    case MonsterType::KNIGHT:
      break;
    case MonsterType::MUSHROOM:
      break;
    case MonsterType::SKEL_ARCHER:
      break;
    case MonsterType::SKEL_SHIELD:
      break;
    case MonsterType::SNAKE:
      break;
  }
}
void Client::UpdateMonsters(UDP_MonsterUpdate* data) noexcept {
  //TODO optimize
  for (auto m : MONSTERS) {
    if (m->u_id == data->monster_id) {
      m->update_state(data);
    }
  }
}
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
