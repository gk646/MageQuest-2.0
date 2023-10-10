#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../ui/game/HealthBar.h"
struct Monster : public Entity {
  EntityStats stats;
  int16_t attack_cd = 0;
  int16_t attack = 0;
  std::string name;
  StatusEffectHandler status_effects{stats};
  ThreatManager threatManager{*this};
  MonsterResource* resource;
  Entity* target = nullptr;
  uint16_t u_id = MONSTER_ID++;
  bool moving = false;
  bool prev_moving = false;
  bool flip = false;
  uint8_t AttackRange = 10;
  uint8_t ChaseRange = 15;
  MonsterType type;
  HealthBar health_bar;
  Monster(const Point& pos, const EntityStats& stats, MonsterResource* resource,
          MonsterType type, const Point& size = {50, 50},
          ShapeType shape_type = ShapeType::RECT)
      : Entity(pos, size, shape_type),
        health_bar((int)size.x_),
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
  ~Monster() override { XPBar::AddPlayerExperience(stats.level); }
#define MONSTER_UPDATE()                                         \
  ENTITY_UPDATE()                                                \
  sprite_counter++;                                              \
  health_bar.update();                                           \
  status_effects.Update();                                       \
  CheckForDeath();                                               \
  if (MP_TYPE == MultiplayerType::CLIENT || attack != 0) return; \
  flip = pos.x_ + size.x_ / 2 > MIRROR_POINT;                    \
  threatManager.Update();                                        \
  attack_cd -= attack == 0;                                      \
  moving = false;

  void draw() override = 0;
  void hit(Projectile& p) noexcept {
    if (p.from_player && p.IsActive() && attack != -100) {
      health_bar.hit();
      status_effects.AddEffects(p.status_effects);
      float dmg = stats.take_damage(p.damage_stats);
      threatManager.AddThreat(p.Sender, dmg);
      p.dead = p.hitType == HitType::ONE_HIT;
    }
  }
  inline void CheckForDeath() noexcept {
    if (stats.health <= 0 && attack != -100) {
      MonsterDiedCallback();
      sprite_counter = 0;
      attack = -100;
    }
  }
  bool WalkToEntity(const Entity* ent) noexcept {
    if (attack != 0) return false;
    PointI point;

    if ((point = PathFinding::AStarPathFinding(tile_pos, ent->tile_pos)) > 0) [[likely]] {
      decideMovement(point, stats.get_speed());
      moving = true;
      return false;
    } else if (point == 0 && !this->intersects(*ent)) {
      float speed = stats.get_speed();
      moving = true;
      if (pos.x_ < ent->pos.x_) {
        pos.x_ += speed;
      } else if (pos.x_ > ent->pos.x_) {
        pos.x_ -= speed;
      } else if (pos.y_ > ent->pos.y_) {
        pos.y_ -= speed;
      } else if (pos.y_ < ent->pos.y_) {
        pos.y_ += speed;
      }
    }
    return true;
  }
  inline void update_state(UDP_MonsterUpdate* data) noexcept {
    if (stats.health != (float)data->new_health) {
      stats.health = data->new_health;
      health_bar.hit();
    }

    if ((attack == 0 && sprite_counter > 100) || data->action_state == -100) {
      sprite_counter = 0;
      attack = data->action_state;
    }

    flip = ((float)data->x < pos.x_);
    if ((int)pos.x_ == data->x && (int)pos.y_ == data->y) {
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
  inline void AttackPlayer3Attacks() noexcept {
    if (attack == 0 && attack_cd <= 0) {
      int num = RANGE_100(RNG_ENGINE);
      attack_cd = 160;
      sprite_counter = 0;
      if (num < 33) {
        attack = 1;
      } else if (num < 66) {
        attack = 2;
      } else {
        attack = 3;
      }
    }
  }
  inline void MonsterDiedCallback() noexcept;
  inline static Monster* GetMonster(float x, float y, MonsterType type,
                                    int level) noexcept;
};

#include "monsters/SkeletonSpear.h"
#include "monsters/SkeletonWarrior.h"
#include "monsters/Wolf.h"
#include "monsters/BloodHound.h"
#include "monsters/Ghost.h"

Monster* Monster::GetMonster(float x, float y, MonsterType type, int level) noexcept {
  switch (type) {
    case MonsterType::SKEL_WAR:
      return new SkeletonWarrior({x, y}, level);
    case MonsterType::ANY:
      break;
    case MonsterType::SKEL_SPEAR:
      return new SkeletonSpear({x, y}, level);
    case MonsterType::WOLF:
      return new Wolf({x, y}, level);
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
      return new BloodHound({x, y}, level);
      //TODO new monsters
      //TODO finish blood hound texture
    case MonsterType::SKEL_ARCHER:
      return new SkeletonSpear({x, y}, level);
      break;
    case MonsterType::SKEL_SHIELD:
      break;
    case MonsterType::SNAKE:
      return new BloodHound({x, y}, level);
    case MonsterType::GHOST:
      break;
    case MonsterType::BLOOD_HOUND:
      return new BloodHound({x, y}, level);
  }
  return nullptr;
}

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
          new UDP_MonsterUpdate(
              m->u_id, static_cast<uint16_t>(m->pos.x_), static_cast<uint16_t>(m->pos.y_),
              (uint16_t)m->stats.health, static_cast<int8_t>(m->attack)),
          sizeof(UDP_MonsterUpdate));
    }
  }
}
void Multiplayer::HandleMonsterSpawn(UDP_MonsterSpawn* data) noexcept {
  auto ptr = Monster::GetMonster(data->x, data->y, data->type, data->level);
  ptr->u_id = data->monster_id;
  MONSTERS.push_back(ptr);
}
void Client::UpdateMonsters(UDP_MonsterUpdate* data) noexcept {
  //TODO optimize
  for (auto m : MONSTERS) {
    if (m->u_id == data->monster_id) {
      m->update_state(data);
    }
  }
}
void ThreatManager::Update() noexcept {
  if (TargetCount > 0) {
    for (auto& te : targets) {
      if (te.entity && te.entity->tile_pos.dist(Self.tile_pos) > Self.ChaseRange) {
        te.threat -= std::max(te.threat * THREAT_DROP, 1.0F);
        if (te.threat <= 0) {
          RemoveTarget(te.entity);
        }
      }
    }
  } else {
    if (PLAYER.tile_pos.dist(Self.tile_pos) <= Self.AttackRange) {
      AddTarget(&PLAYER, PLAYER_STATS.level);
    }

    for (const auto np : OTHER_PLAYERS) {
      if (np) {
        if (np->tile_pos.dist(Self.tile_pos) <= Self.AttackRange) {
          AddTarget(np, np->stats.level);
        }
      }
    }
  }
}

void SpawnTrigger::Trigger() noexcept {
  if (triggered) return;
  triggered = true;

  if (level == 0) level = PLAYER_STATS.level;
  if (isSingular) {
    MONSTERS.push_back(Monster::GetMonster(pos.x, pos.y, type, level));
  } else {
  }
}
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
