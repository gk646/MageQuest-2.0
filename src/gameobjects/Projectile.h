#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
struct Projectile : public Entity {
  Sound sound;
  std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> statusEffects;
  const ProjectileResources* resources = nullptr;
  const Entity* sender = nullptr;
  DamageStats damageStats;
  Vector2 mvmVector;
  float speed;
  int16_t lifeSpanTicks;
  uint16_t spriteCounter = 0;
  HitType hitType = HitType::ONE_HIT;
  bool isFriendlyToPlayer;
  bool isDoingDamage = true;
  ProjectileType projectileType = ProjectileType::FIRE_BALL;
  Projectile(bool isFriendlyToPlayer, const Point& pos, const DamageStats& damage_stats,
             const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
             const Vector2& move_vector, int16_t pov, const Sound& sourceSound,
             const Entity* sender, ProjectileType pType) noexcept
      : Entity(pos, typeToInfo[pType].size, typeToInfo[pType].hitBoxShape, pov,
               typeToInfo[pType].illuminated),
        lifeSpanTicks(typeToInfo[pType].lifeSpan),
        speed(typeToInfo[pType].speed),
        damageStats(damage_stats),
        isFriendlyToPlayer(isFriendlyToPlayer),
        hitType(typeToInfo[pType].hitType),
        statusEffects(effects),
        mvmVector(move_vector),
        resources(typeToInfo[pType].res),
        projectileType(pType),
        sound(LoadSoundAlias(sourceSound)),
        sender(sender) {
    PlaySoundR(sound);
  }
  Projectile(const Projectile& p) noexcept
      : Entity(p),
        mvmVector(p.mvmVector),
        speed(p.speed),
        lifeSpanTicks(p.lifeSpanTicks),
        damageStats(p.damageStats),
        isFriendlyToPlayer(p.isFriendlyToPlayer),
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
    isFriendlyToPlayer = other.isFriendlyToPlayer;
    hitType = other.hitType;

    return *this;
  }
  ~Projectile() noexcept override {
    StopSound(sound);
    UnloadSoundAlias(sound);
    for (auto ptr : statusEffects) {
      delete ptr;
    }
  }
  inline Vector2 GetMovementVectorToMouse() noexcept {
    float angle = std::atan2(MOUSE_POS.y - pos.y_ - DRAW_Y - size.y / 2,
                             MOUSE_POS.x - pos.x_ - DRAW_X - size.x / 2);
    return {std::cos(angle), std::sin(angle)};
  }
  inline Vector2 GetMovementVectorToWorldPos(const Point& worldPos) {
    float angle =
        std::atan2(worldPos.y_ - pos.y_ - size.y / 2, worldPos.x_ - pos.x_ - size.x / 2);
    pov = angle * RAD2DEG;
    return {std::cos(angle), std::sin(angle)};
  }
  [[nodiscard]] inline bool IsActive() const noexcept { return !isDead && isDoingDamage; }
  inline void Update() override {
    spriteCounter++;
    pos.x_ += mvmVector.x * speed;
    pos.y_ += mvmVector.y * speed;
    tilePos.x = (pos.x_ + size.x / 2) / TILE_SIZE;
    tilePos.y = (pos.y_ + size.y / 2) / TILE_SIZE;
    if (!BoundCheckMap(tilePos.x, tilePos.y) ||
        CheckTileCollision(tilePos.x, tilePos.y)) {
      HitWallCallback();
    }
    lifeSpanTicks--;
    if (lifeSpanTicks <= 0) {
      isDead = true;
    }
  }
  virtual void HitTargetCallback() noexcept { isDead = hitType == HitType::ONE_HIT; }
  virtual void HitWallCallback() noexcept { isDead = true; }
};

#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
