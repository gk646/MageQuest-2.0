#ifndef MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
#define MAGE_QUEST_SRC_ENTITIES_MONSTER_H_

#include "../ui/game/HealthBar.h"
#include "components/HealthDropComponent.h"

#define MONSTER_UPDATE()                                                              \
  ENTITY_UPDATE()                                                                     \
  spriteCounter++;                                                                    \
  healthBar.update();                                                                 \
  effectHandler.Update();                                                             \
  CheckForDeath();                                                                    \
  if (MP_TYPE == MultiplayerType::CLIENT) return;                                     \
  hitFlashDuration = std::max(-12, hitFlashDuration - 1);                             \
  isFlipped = pos.x_ + size.x / 2.0F > MIRROR_POINT && threatManager.targetCount > 0; \
  attackComponent.Update();                                                           \
  if (actionState != 0) return;                                                       \
  threatManager.Update();                                                             \
  isMoving = false;

struct Monster : public Entity {
  EntityStats stats;
  ThreatManager threatManager{this};
  AttackComponent attackComponent;
  StatusEffectHandler effectHandler{stats, this};
  const MonsterResource* resource;
  HealthBar healthBar;
  uint16_t u_id = MONSTER_ID++;
  bool isMoving = false;
  bool prevMoveState = false;
  bool isFlipped = false;
  int8_t hitFlashDuration = 0;
  MonsterType type;
  Monster(const Point& pos, const MonsterScaler& scaler, uint8_t level,
          const MonsterResource* resourceArg, MonsterType typeArg,
          const PointT<int16_t>& size, Zone zone, ShapeType hitboxShape = ShapeType::RECT)
      : Entity(pos, size, hitboxShape),
        stats({scaler, level}),
        resource(resourceArg),
        attackComponent(this, scaler),
        type(typeArg),
        healthBar((int)size.x) {
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
        effectHandler(stats, this),
        resource(other.resource),
        healthBar(other.healthBar),
        attackComponent(other.attackComponent),
        type(other.type) {}
  Monster& operator=(const Monster& other) {
    if (this != &other) {
      Entity::operator=(other);
      stats = other.stats;
      threatManager = other.threatManager;
      effectHandler = other.effectHandler;
      resource = other.resource;
      healthBar = other.healthBar;
      type = other.type;
    }
    return *this;
  }
  ~Monster() override { XPBar::AddPlayerExperience(stats.level); }
  inline void Hit(Projectile& p) noexcept {
    if (p.isFriendlyToPlayer && p.IsActive() && actionState != -100 &&
        !IsHitDodged(stats)) {
      healthBar.Update();
      p.HitTargetCallback();
      const float dmg = stats.TakeDamage(p.damageStats, this);
      threatManager.AddThreat(p.sender, dmg);
      effectHandler.AddEffects(p.statusEffects);
      hitFlashDuration = hitFlashDuration == -12 ? 12 : hitFlashDuration;
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
    if ((point = PathFinding::AStarPathFinding(tilePos, ent->tilePos)) > 0) [[likely]] {
      CalculateMovement(point, speed);
      isMoving = true;
      return false;
    } else if (point == 0) {
      if (!this->Intersects(*ent)) {
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
  inline bool WalkCloseToEntity(const Entity* ent, float distance) noexcept {
    if (actionState != 0) return false;
    if (ent->tilePos.dist(tilePos) <= distance) return true;
    PointT<int16_t> point;
    float speed = stats.GetSpeed();
    if ((point = PathFinding::AStarPathFinding(tilePos, ent->tilePos)) > 0) [[likely]] {
      CalculateMovement(point, speed);
      isMoving = true;
      return false;
    } else if (point == 0) {
      if (!this->Intersects(*ent)) {
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
  inline void UpdateWithRemoteState(const UDP_MonsterUpdate* data) noexcept {
    if (stats.health != (float)data->new_health) {
      stats.health = data->new_health;
      healthBar.Update();
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
  inline void MonsterDiedCallback() noexcept;
  inline static Monster* GetNewMonster(const Point& pos, MonsterType type, uint8_t level,
                                       Zone zone = CURRENT_ZONE) noexcept;
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
};

#include "monsters/Monsters.h"

Monster* Monster::GetNewMonster(const Point& pos, MonsterType type, uint8_t level,
                                Zone zone) noexcept {
  switch (type) {
    case MonsterType::SKEL_WAR:
      return new SkeletonWarrior(pos, level, type, zone);
    case MonsterType::SKEL_SPEAR:
      return new SkeletonSpear(pos, level, type, zone);
    case MonsterType::WOLF:
      return new Wolf(pos, level, type, zone);
    case MonsterType::MUSHROOM:
      return new FangShroom(pos, level, type, zone);
    case MonsterType::SKEL_ARCHER:
      return new SkeletonArcher(pos, level, type, zone);
    case MonsterType::SKEL_SHIELD:
      return new SkeletonShield(pos, level, type, zone);
    case MonsterType::SNAKE:
      return new Snake(pos, level, type, zone);
    case MonsterType::GHOST:
      return new Ghost(pos, level, type, zone);
    case MonsterType::BLOOD_HOUND:
      return new BloodHound(pos, level, type, zone);
    case MonsterType::SKULL_WOLF:
      return new SkullWolf(pos, level, type, zone);
    case MonsterType::BOSS_ANCIENT_GOLEM:
      return new BossStoneGolem(pos, level, type, zone);
    case MonsterType::RAT:
      return new Rat(pos, level, type, zone);
    case MonsterType::GOBLIN:
      return new Goblin(pos, level, type, zone);
    case MonsterType::BOSS_DEATH_BRINGER:
    case MonsterType::BOSS_STONE_KNIGHT:
    case MonsterType::BOSS_SLIME:
    case MonsterType::KNIGHT:
    case MonsterType::ANY:
      break;
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
    if (m->isMoving || m->healthBar.delay > 0 || m->actionState != 0) {
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
  auto ptr =
      Monster::GetNewMonster({(float)data->x, (float)data->y}, data->type, data->level);
  ptr->u_id = data->monster_id;
  MONSTERS.push_back(ptr);
}
void Client::UpdateMonsters(const UDP_MonsterUpdate* data) noexcept {
  SIMD_PRAGMA
  for (const auto& m : MONSTERS) {
    if (m->u_id == data->monster_id) {
      m->UpdateWithRemoteState(data);
    }
  }
}

void ThreatManager::Update() noexcept {
  if (targetCount > 0) {
    for (auto& te : targets) {
      if (te.entity && te.entity->tilePos.dist(self->tilePos) >
                           self->attackComponent.chaseRangeTiles) {
        te.threat -= std::max(te.threat * THREAT_DROP, 1.0F);
        if (te.threat <= 0) {
          RemoveTarget(te.entity);
        }
      }
    }
  } else {
    if (PLAYER.tilePos.dist(self->tilePos) <= self->attackComponent.attackRangeTiles) {
      AddTarget(&PLAYER, PLAYER_STATS.level);
    }

    for (const auto np : OTHER_PLAYERS) {
      if (np) {
        if (np->tilePos.dist(self->tilePos) <= self->attackComponent.attackRangeTiles) {
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
    MONSTERS.push_back(Monster::GetNewMonster({(float)pos.x, (float)pos.y}, type, level));
  } else {
    //TODO area triggers
  }
}

//Attack Component
void AttackComponent::StartAnimation(int8_t actionState) const noexcept {
  self->spriteCounter = 0;
  self->actionState = actionState;
}
bool CustomAbility::IsReady(Monster* self) noexcept {
  return currentCooldown == 0 && currentDelay == -1 &&
         ((range > 0 && PLAYER.tilePos.dist(self->tilePos) > range) ||
          (range < 0 && PLAYER.tilePos.dist(self->tilePos) < std::abs(range)));
}
bool ProjectileAttack::IsReady(Monster* self) noexcept {
  return currentCooldown == 0 && currentDelay == -1 &&
         ((range > 0 && PLAYER.tilePos.dist(self->tilePos) > range) ||
          (range < 0 && PLAYER.tilePos.dist(self->tilePos) < std::abs(range)));
}
void ProjectileAttack::Execute(Monster* attacker) const {
  std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> copy{};
  for (uint_fast32_t i = 0; i < MAX_STATUS_EFFECTS_PRJ; i++) {
    if (!effects[i]) break;
    copy[i] = effects[i]->Clone();
  }
  PROJECTILES.emplace_back(GetProjectileInstance(type, attacker->GetMiddlePoint(), false,
                                                 damage * modifier, attacker, Vector2(), 0, copy));
}
void ConeAttack::Execute(Monster* attacker) const {
  std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> copy{};
  for (uint_fast32_t i = 0; i < MAX_STATUS_EFFECTS_PRJ; i++) {
    if (!effects[i]) break;
    copy[i] = effects[i]->Clone();
  }
  auto prj = new AttackCone(attacker->GetAttackConeBounds(width, height), false,
                            (int16_t)std::max(hitDelay * 2, 90), hitDelay, damage * modifier, copy,
                            sound, attacker);
  SetDamageStats(prj, attacker->stats.effects[CRIT_CHANCE],
                 attacker->stats.effects[CRIT_DAMAGE_P]);
  PROJECTILES.emplace_back(prj);
}
#endif  //MAGE_QUEST_SRC_ENTITIES_MONSTER_H_
