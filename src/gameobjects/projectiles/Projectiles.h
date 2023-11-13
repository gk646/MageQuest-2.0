#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PROJECTILES_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PROJECTILES_H_

struct BlastHammer final : Projectile {
  bool flip;
  BlastHammer(const Point& pos, bool isFriendlyToPlayer, float damage,
              const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
              const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::FIRE, damage}, effects, {0, 0},
                   0, sound::blastHammer, sender, BLAST_HAMMER),
        flip(pos.x_ < MIRROR_POINT) {
    isDoingDamage = false;
  }
  void Draw() final {
    if (spriteCounter < 110) {
      DrawTextureProFastEx(resources->frames[spriteCounter % 110 / 10],
                           pos.x_ + DRAW_X - 67, pos.y_ + DRAW_Y - 95, -87, 0, flip,
                           WHITE);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == (int16_t)typeToInfo[projectileType].val1) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct EnergySphere final : Projectile {
  EnergySphere(const Point& pos, bool isFriendlyToPlayer, float damage,
               const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
               const Vector2& mvmt, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::ARCANE, damage}, effects, mvmt,
                   0, sound::energySphere, sender, ENERGY_SPHERE) {}
  void Draw() final {
    DrawTextureProFast(resources->frames[spriteCounter % 42 / 7], pos.x_ + DRAW_X - 8,
                       pos.y_ + DRAW_Y - 8, 0, WHITE);
    DRAW_HITBOXES();
  }
};
struct FireBall final : Projectile {
  FireBall(const Point& pos, bool isFriendlyToPlayer, float damage,
           const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects, int16_t pov,
           const Vector2& mvmt, const Entity* sender, bool sound = false)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::FIRE, damage}, effects, mvmt,
                   pov, sound ? sound::EMPTY_SOUND : sound::fireBurst, sender,
                   FIRE_BALL) {}
  void Draw() final {
    DrawTextureProFastRotOffset(resources->frames[spriteCounter % 135 / 3],
                                pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -58, -22);
    DRAW_HITBOXES();
  }
};
struct PsychicScream final : Projectile {
  PsychicScream(const Point& pos, bool isFriendlyToPlayer, float damage)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::DARK, damage}, {new Stun(120)},
                   {0, 0}, 0, sound::speak, nullptr, PSYCHIC_SCREAM) {
    isDoingDamage = false;
  }
  void Draw() final {
    DrawTextureProFast(resources->frames[spriteCounter % 60 / 6], pos.x_ + DRAW_X - 57,
                       pos.y_ + DRAW_Y - 50, 0, WHITE);
    DRAW_HITBOXES();
  }
  void Update() final {
    Projectile::Update();
    if (lifeSpanTicks == (int16_t)typeToInfo[projectileType].val1) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct Lightning final : Projectile {
  bool flip;
  Lightning(const Point& pos, bool isFriendlyToPlayer, float damage,
            const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
            const Vector2& mvmt, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::ARCANE, damage}, effects, mvmt,
                   0, sound::lightning, sender, LIGHTNING),
        flip(pos.x_ < MIRROR_POINT) {}
  void Draw() final {
    if (Projectile::spriteCounter < 70) {
      DrawTextureProFastEx(resources->frames[spriteCounter % 70 / 7],
                           pos.x_ + DRAW_X - 10, pos.y_ + DRAW_Y - 53, 0, 0, flip, WHITE);
    }

    DRAW_HITBOXES();
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == (int16_t)typeToInfo[projectileType].val1) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct FrostNova final : Projectile {
  FrostNova(const Point& pos, bool isFriendlyToPlayer, float damage,
            const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
            const Vector2& mvmt, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::ICE, damage}, effects, mvmt, 0,
                   sound::frostNova, sender, FROST_NOVA) {
    AddStatusEffect(this, new Root(120));
  }

  void Draw() final {
    if (spriteCounter > 55) return;
    DrawTextureProFastEx(resources->frames[spriteCounter % 56 / 2], pos.x_ + DRAW_X - 50,
                         pos.y_ + DRAW_Y - 50, 0, 0, false, WHITE);

    DRAW_HITBOXES();
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == (int16_t)typeToInfo[projectileType].val1) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct GlacialBurst final : Projectile {
  GlacialBurst(const Point& pos, bool isFriendlyToPlayer, float damage,
            const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
            const Vector2& mvmt, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::ICE, damage}, effects, mvmt, 0,
                   sound::glacialBurst, sender, GLACIAL_BURST) {
    AddStatusEffect(this, new Root(120));
  }

  void Draw() final {
    DrawTextureProFastEx(resources->frames[spriteCounter % 266 / 7], pos.x_ + DRAW_X -10,
                         pos.y_ + DRAW_Y-21, 0, 0, false, WHITE);

    DRAW_HITBOXES();
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == (int16_t)typeToInfo[projectileType].val1) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct AttackCone final : Projectile {
  uint16_t hitDelay = 0;
  AttackCone(const RectangleR& rect, bool isFriendlyToPlayer, int16_t life_span,
             int16_t hitDelay, float damage,
             const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
             const Sound& sound, const Entity* sender)
      : Projectile(isFriendlyToPlayer, {rect.x, rect.y}, {DamageType::PHYSICAL, damage},
                   effects, {0, 0}, 0, sound, sender, PROJECTILE_END),
        hitDelay(hitDelay) {
    hitType = HitType::ONE_TICK;
    lifeSpanTicks = life_span;
    size = {static_cast<int16_t>(rect.width), static_cast<int16_t>(rect.height)};
  }
  void Draw() final { DRAW_HITBOXES(); }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == hitDelay) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct ArrowNormal final : Projectile {
  ArrowNormal(const Point& pos, bool isFriendlyToPlayer, float damageValue,
              const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& statusEffects,
              const Vector2& mvmt, const Sound& sound, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::PHYSICAL, damageValue},
                   statusEffects, mvmt, 0, sound, sender, ARROW_NORMAL) {
    if (!isFriendlyToPlayer) {
      this->mvmVector = GetMovementVectorToWorldPos({PLAYER_X + 15, PLAYER_Y + 24});
    }
  }
  void Draw() final {
    DrawTextureProFastRotOffset(resources->frames[spriteCounter % 20 / 2],
                                pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, 0, 0);

    DRAW_HITBOXES();
  }
};
//Flies normally expect when hitting anything // Does damage for 1 tick and then explodes
struct ArcaneBolt final : Projectile {
  bool hitTarget = false;
  ArcaneBolt(const Point& pos, bool isFriendlyToPlayer, float damage,
             const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects,
             int16_t pov, const Vector2& mvmt, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::ARCANE, damage}, effects, mvmt,
                   (int16_t)pov, sound::fireBurst, sender, ARCANE_BOLT) {
    projectileType = ProjectileType::ARCANE_BOLT;
    isIlluminated = true;
    isDoingDamage = true;
  }
  void Draw() final {
    if (!hitTarget) {
      DrawTextureProFastRotOffset(resources->frames[spriteCounter % 60 / 15],
                                  pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -12, -50);
    } else {
      DrawTextureProFastRotOffset(resources->frames[spriteCounter % 117 / 13],
                                  pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -15, -50);
    }
    DRAW_HITBOXES();
  }
  void Update() noexcept final {
    Projectile::Update();
    if (hitTarget) {
      isDoingDamage = false;
    }
  }
  void HitTargetCallback() noexcept final {
    if (!hitTarget) {
      mvmVector.x = 0;
      mvmVector.y = 0;
      hitTarget = true;
      spriteCounter = 78;
      lifeSpanTicks = 40;
    }
  }
  void HitWallCallback() noexcept final {
    if (!hitTarget) {
      mvmVector.x = 0;
      mvmVector.y = 0;
      hitTarget = true;
      spriteCounter = 78;
      lifeSpanTicks = 40;
    }
  }
};
struct IceLance final : Projectile {
  bool hitTarget = false;
  IceLance(const Point& pos, bool isFriendlyToPlayer, float damage,
           const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects, int16_t pov,
           const Vector2& mvmt, const Entity* sender)
      : Projectile(isFriendlyToPlayer, pos, {DamageType::ICE, damage}, effects, mvmt,
                   (int16_t)pov, sound::iceLance, sender, ICE_LANCE) {}

  void Draw() final {
    if (!hitTarget) {
      DrawTextureProFastRotOffset(resources->frames[spriteCounter % 104 / 8],
                                  pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -5, -7);
    } else {
      DrawTextureProFastRotOffset(resources->frames[spriteCounter % 160 / 8],
                                  pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -5, -7);
    }
    DRAW_HITBOXES();
  }
  void Update() noexcept final {
    Projectile::Update();
    if (hitTarget) {
      isDoingDamage = false;
    }
  }
  void HitTargetCallback() noexcept final {
    if (!hitTarget) {
      mvmVector.x = 0;
      mvmVector.y = 0;
      hitTarget = true;
      spriteCounter = 104;
      lifeSpanTicks = 56;
    }
  }
  void HitWallCallback() noexcept final {
    if (!hitTarget) {
      mvmVector.x = 0;
      mvmVector.y = 0;
      hitTarget = true;
      spriteCounter = 104;
      lifeSpanTicks = 56;
    }
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PROJECTILES_H_
