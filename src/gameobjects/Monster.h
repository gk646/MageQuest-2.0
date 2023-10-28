#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../ui/game/HealthBar.h"

#define GENERATE_DRAW_ATTACK(ATTACK_NAME, PICTURES_TOTAL, DELAY_TOTAL, OFFSET_X,   \
                             OFFSET_Y)                                             \
  constexpr int step_##ATTACK_NAME = DELAY_TOTAL / PICTURES_TOTAL;                 \
  inline void draw_##ATTACK_NAME() noexcept {                                      \
    int num = spriteCounter % DELAY_TOTAL / step_##ATTACK_NAME;                    \
    if (num < PICTURES_TOTAL) {                                                    \
      DrawTextureProFastEx(resource->ATTACK_NAME[num], pos.x_ + DRAW_X + OFFSET_X, \
                           pos.y_ + DRAW_Y + OFFSET_Y, 0, 0, isFlipped, WHITE);    \
    } else {                                                                       \
      actionState = 0;                                                             \
    }                                                                              \
  }

#define MONSTER_UPDATE()                             \
  ENTITY_UPDATE()                                    \
  spriteCounter++;                                   \
  health_bar.update();                               \
  effectHandler.Update();                            \
  CheckForDeath();                                   \
  if (MP_TYPE == MultiplayerType::CLIENT) return;    \
  isFlipped = pos.x_ + size.x / 2.0F > MIRROR_POINT; \
  if (actionState != 0) return;                      \
  threatManager.Update();                            \
  attackStats.Update(actionState);                   \
  isMoving = false;

struct Monster : public Entity {
  EntityStats stats;
  ThreatManager threatManager{this};
  StatusEffectHandler effectHandler{stats};
  const MonsterResource* resource;
  MonsterAttackStats attackStats;
  HealthBar health_bar;
  uint16_t u_id = MONSTER_ID++;
  int8_t actionState = 0;
  bool isMoving = false;
  bool prevMoveState = false;
  bool isFlipped = false;
  MonsterType type;
  Monster(const Point& pos, const MonsterScaler& scaler, uint8_t level,
          const MonsterResource* resourceArg, MonsterType typeArg,
          const PointT<int16_t>& size, ShapeType hitboxShape = ShapeType::RECT)
      : Entity(pos, size, hitboxShape),
        stats({scaler, level}),
        resource(resourceArg),
        attackStats(scaler),
        type(typeArg),
        health_bar((int)size.x) {
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
        threatManager(this),
        effectHandler(stats),
        resource(other.resource),
        health_bar(other.health_bar),
        attackStats(other.attackStats),
        type(other.type) {}
  Monster& operator=(const Monster& other) {
    if (this != &other) {
      Entity::operator=(other);
      stats = other.stats;
      threatManager = other.threatManager;
      effectHandler = other.effectHandler;
      resource = other.resource;
      health_bar = other.health_bar;
      type = other.type;
    }
    return *this;
  }
  ~Monster() override { XPBar::AddPlayerExperience(stats.level); }
  void Draw() override = 0;
  void Hit(Projectile& p) noexcept {
    if (p.from_player && p.IsActive() && actionState != -100) {
      p.HitTargetCallback();
      health_bar.Update();
      effectHandler.AddEffects(p.statusEffects);
      float dmg = stats.TakeDamage(p.damageStats);
      threatManager.AddThreat(p.sender, dmg);
    }
  }
  inline void CheckForDeath() noexcept {
    if (stats.health <= 0 && actionState != -100) {
      MonsterDiedCallback();
      spriteCounter = 0;
      actionState = -100;
    }
  }
  inline bool WalkToEntity(const Entity* ent) noexcept {
    if (actionState != 0) return false;
    PointT<int16_t> point;
    float speed = stats.GetSpeed();
    if ((point = PathFinding::AStarPathFinding(tile_pos, ent->tile_pos)) > 0) [[likely]] {
      CalculateMovement(point, speed);
      isMoving = true;
      return false;
    } else if (point == 0) {
      if (!this->intersects(*ent)) {
        isMoving = true;
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
    return false;
  }
  inline bool WalkCloseToEntity(const Entity* ent, int distance) noexcept {
    if (actionState != 0) return false;
    if (ent->tile_pos.dist(tile_pos) <= distance) return true;
    PointT<int16_t> point;
    float speed = stats.GetSpeed();
    if ((point = PathFinding::AStarPathFinding(tile_pos, ent->tile_pos)) > 0) [[likely]] {
      CalculateMovement(point, speed);
      isMoving = true;
      return false;
    } else if (point == 0) {
      if (!this->intersects(*ent)) {
        isMoving = true;
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
    return false;
  }
  inline void UpdateWithRemoteState(UDP_MonsterUpdate* data) noexcept {
    if (stats.health != (float)data->new_health) {
      stats.health = data->new_health;
      health_bar.Update();
    }

    if ((actionState == 0 && spriteCounter > 100) || data->action_state == -100) {
      spriteCounter = 0;
      actionState = data->action_state;
    }

    isFlipped = ((float)data->x < pos.x_);
    if ((int)pos.x_ == data->x && (int)pos.y_ == data->y) {
      if (prevMoveState) {
        prevMoveState = false;
        return;
      }
      isMoving = false;
    } else {
      isMoving = true;
      prevMoveState = true;
    }
    pos.x_ = data->x;
    pos.y_ = data->y;
  }
  inline bool AttackPlayer3Attacks() noexcept {
    if (attackStats.IsAttackReady(actionState)) {
      int num = RANGE_100(RNG_ENGINE);
      attackStats.ResetCooldown();
      spriteCounter = 0;
      if (num < 33) {
        actionState = 1;
      } else if (num < 66) {
        actionState = 2;
      } else {
        actionState = 3;
      }
      return true;
    }
    return false;
  }
  inline void MonsterDiedCallback() noexcept;
  inline static Monster* GetNewMonster(float x, float y, MonsterType type,
                                       uint8_t level) noexcept;
  inline RectangleR GetAttackConeBounds(int attackWidth, int attackHeight) noexcept {
    RectangleR ret = {0};
    auto playerPos = PLAYER.pos;
    ret.width = attackWidth;
    ret.height = attackHeight;
    ret.y = playerPos.y_ < pos.y_ ? pos.y_ - size.y / 3 : pos.y_ + size.y / 3;

    if (isFlipped) {
      ret.x = pos.x_ - ret.width;
    } else {
      ret.x = pos.x_ + size.x / 3;
    }
    ret.width += size.x;
    return ret;
  }
  inline void SetBaseValues(const MonsterScaler& scaler) noexcept {}
};

#include "monsters/SkeletonSpear.h"
#include "monsters/SkeletonWarrior.h"
#include "monsters/SkeletonArcher.h"
#include "monsters/Wolf.h"
#include "monsters/BloodHound.h"
#include "monsters/Ghost.h"

Monster* Monster::GetNewMonster(float x, float y, MonsterType type,
                                uint8_t level) noexcept {
  switch (type) {
    case MonsterType::SKEL_WAR:
      return new SkeletonWarrior({x, y}, level, type);
    case MonsterType::ANY:
      break;
    case MonsterType::SKEL_SPEAR:
      return new SkeletonSpear({x, y}, level, type);
    case MonsterType::WOLF:
      return new Wolf({x, y}, level, type);
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
      return new BloodHound({x, y}, level, type);
    case MonsterType::SKEL_ARCHER:
      return new SkeletonArcher({x, y}, level, type);
      break;
    case MonsterType::SKEL_SHIELD:
      break;
    case MonsterType::SNAKE:
      return new BloodHound({x, y}, level, type);
    case MonsterType::GHOST:
      return new Ghost({x, y}, level, type);
      break;
    case MonsterType::BLOOD_HOUND:
      return new BloodHound({x, y}, level, type);
  }
  std::cout << "MISSING MONSTER ID AT ENUM ID:" << (int)type << std::endl;
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
    if (m->isMoving || m->health_bar.delay > 0 || m->actionState != 0) {
      Server::SendMsgToAllUsers(
          UDP_MONSTER_UPDATE,
          new UDP_MonsterUpdate(
              m->u_id, static_cast<uint16_t>(m->pos.x_), static_cast<uint16_t>(m->pos.y_),
              (uint16_t)m->stats.health, static_cast<int8_t>(m->actionState)),
          sizeof(UDP_MonsterUpdate));
    }
  }
}
void Multiplayer::HandleMonsterSpawn(UDP_MonsterSpawn* data) noexcept {
  auto ptr = Monster::GetNewMonster(data->x, data->y, data->type, data->level);
  ptr->u_id = data->monster_id;
  MONSTERS.push_back(ptr);
}
void Client::UpdateMonsters(UDP_MonsterUpdate* data) noexcept {
  //TODO optimize
  for (auto m : MONSTERS) {
    if (m->u_id == data->monster_id) {
      m->UpdateWithRemoteState(data);
    }
  }
}

void ThreatManager::Update() noexcept {
  if (TargetCount > 0) {
    for (auto& te : targets) {
      if (te.entity &&
          te.entity->tile_pos.dist(self->tile_pos) > self->attackStats.chaseRange) {
        te.threat -= std::max(te.threat * THREAT_DROP, 1.0F);
        if (te.threat <= 0) {
          RemoveTarget(te.entity);
        }
      }
    }
  } else {
    if (PLAYER.tile_pos.dist(self->tile_pos) <= self->attackStats.attackRange) {
      AddTarget(&PLAYER, PLAYER_STATS.level);
    }

    for (const auto np : OTHER_PLAYERS) {
      if (np) {
        if (np->tile_pos.dist(self->tile_pos) <= self->attackStats.attackRange) {
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
    MONSTERS.push_back(Monster::GetNewMonster(pos.x, pos.y, type, level));
  } else {
  }
}
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
