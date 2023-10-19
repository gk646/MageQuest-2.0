#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_

struct Projectile : public Entity {
  std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> statusEffects;
  const ProjectileResources* resources = nullptr;
  const Entity* sender = nullptr;
  Sound* sound = nullptr;
  DamageStats damageStats;
  Vector2 mvmVector;
  float speed;
  int16_t lifeSpanTicks;
  uint16_t spriteCounter = 0;
  HitType hitType = HitType::ONE_HIT;
  bool from_player;
  bool isDoingDamage = true;
  bool isSoundDoubled = false;
  ProjectileType projectileType = ProjectileType::FIRE_BALL;
  Projectile(bool from_player, const Point& pos, const Point& size, ShapeType shape_type,
             int life_span, float speed, const DamageStats& damage_stats, HitType type,
             const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
             const Vector2& move_vector, float pov, Sound* sound,
             ProjectileResources* res, const Entity* sender) noexcept
      : Entity(pos, size, shape_type, pov),
        lifeSpanTicks(static_cast<int16_t>(life_span)),
        speed(speed),
        damageStats(damage_stats),
        from_player(from_player),
        hitType(type),
        statusEffects(effects),
        sound(sound),
        mvmVector(move_vector),
        resources(res),
        sender(sender) {
    PlaySoundR(*sound);
  }
  Projectile(const Projectile& p) noexcept
      : Entity(p),
        mvmVector(p.mvmVector),
        speed(p.speed),
        lifeSpanTicks(p.lifeSpanTicks),
        damageStats(p.damageStats),
        from_player(p.from_player),
        hitType(p.hitType),
        sound(p.sound),
        statusEffects() {
    for (uint_fast32_t i = 0; i < MAX_STATUS_EFFECTS_PRJ; i++) {
      statusEffects[i] = p.statusEffects[i]->clone();
    }
  }
  Projectile& operator=(const Projectile& other) noexcept {
    if (this == &other) {
      return *this;
    }

    Entity::operator=(other);

    for (auto ptr : statusEffects) {
      delete ptr;
    }

    for (uint_fast32_t i = 0; i < MAX_STATUS_EFFECTS_PRJ; i++) {
      statusEffects[i] = other.statusEffects[i]->clone();
    }

    mvmVector = other.mvmVector;
    speed = other.speed;
    lifeSpanTicks = other.lifeSpanTicks;
    damageStats = other.damageStats;
    from_player = other.from_player;
    hitType = other.hitType;

    return *this;
  }
  ~Projectile() noexcept override {
    StopSound(*sound);
    for (auto ptr : statusEffects) {
      delete ptr;
    }
  }
  inline Vector2 GetMovementVector() noexcept {
    float angle = std::atan2(MOUSE_POS.y - pos.y_ - DRAW_Y - size.y_ / 2,
                             MOUSE_POS.x - pos.x_ - DRAW_X - size.x_ / 2);
    return {std::cos(angle), std::sin(angle)};
  }
  [[nodiscard]] inline bool IsActive() const noexcept { return !dead && isDoingDamage; }
  void Update() override {
    spriteCounter++;
    pos.x_ += mvmVector.x * speed;
    pos.y_ += mvmVector.y * speed;
    tile_pos.x = static_cast<int>(pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = static_cast<int>(pos.y_ + size.y_ / 2) / TILE_SIZE;
    if (!BoundCheckMap(tile_pos.x, tile_pos.y) ||
        CheckTileCollision(tile_pos.x, tile_pos.y)) {
      dead = true;
    }
    lifeSpanTicks--;
    if (lifeSpanTicks <= 0) {
      dead = true;
    }
  }
};

#include "projectiles/FireBall.h"
#include "projectiles/PsychicScream.h"
#include "projectiles/Dummy.h"
#include "projectiles/PoisonBall.h"
#include "projectiles/BlastHammer.h"
#include "projectiles/EnergySphere.h"
#include "projectiles/FireSword.h"
#include "projectiles/FrostNova.h"
#include "projectiles/IceLance.h"
#include "projectiles/InfernoRay.h"
#include "projectiles/Lightning.h"
#include "projectiles/PowerSurge.h"
#include "projectiles/PyroBlast.h"
#include "projectiles/SolarFlare.h"
#include "projectiles/ThunderStrike.h"
#include "projectiles/ThunderSplash.h"
#include "projectiles/VoidEruption.h"
#include "projectiles/VoidField.h"
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
