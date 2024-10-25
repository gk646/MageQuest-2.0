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
  //PVP will be handled on a global context -> either all players pvp or none
  //position has to be given
  //Damage stats are given by the projectile itself + by the caster -> calculated upfront with each hit having the +-5% damage and crit chance
  //effects given by blueprint and by the caster-> at the time of creating snapshotted

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
    ApplyTalentsToProjectile(this);
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
    for (int i = 0; i < MAX_STATUS_EFFECTS_PRJ; i++) {
      statusEffects[i] = p.statusEffects[i]->Clone();
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

    for (int i = 0; i < MAX_STATUS_EFFECTS_PRJ; i++) {
      statusEffects[i] = other.statusEffects[i]->Clone();
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
    for (auto& ptr : statusEffects) {
      delete ptr;
      ptr = nullptr;
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
    if (hitType == HitType::CONTINUOUS) {
      isDoingDamage = spriteCounter % 10 == 0;
    }
    lifeSpanTicks--;
    if (lifeSpanTicks <= 0) {
      isDead = true;
    }
  }
  virtual void HitTargetCallback() noexcept { isDead = hitType == HitType::ONE_HIT; }
  virtual void HitWallCallback() noexcept { isDead = true; }
  //Shoot create projectiles of the given type and send them off in regular intervals of 360/"num" degrees away from the center
  inline static void ShootRadial(const Point& center, int num, ProjectileType type,
                                 float damage, bool randomized = false) noexcept;
};

inline static void AddStatusEffect(Projectile* prj, StatusEffect* effect) noexcept {
  for (auto& e : prj->statusEffects) {
    if (!e) {
      e = effect;
      return;
    }
  }
}
//Sets critChance and critDamage (as percent value e.g. 0.25) for the projectile
inline static void SetDamageStats(Projectile* prj, float critChance,
                                  float critDamage_P) noexcept {
  prj->damageStats.critDamage = critDamage_P;
  prj->damageStats.critChance = critChance;
}
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILE_H_
