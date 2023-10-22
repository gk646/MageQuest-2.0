#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PROJECTILES_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PROJECTILES_H_
struct BlastHammer final : Projectile {
  static constexpr int width = 50;
  static constexpr int height = 50;
  static constexpr int HIT_DELAY = 50;
  bool flip;
  BlastHammer(const Point& pos, bool from_player, int life_span, float speed,
              float damage, HitType hit_type, const std::array<StatusEffect*, 3>& effects,
              float pov, const Vector2& move, bool flip, const Entity* sender)
      : Projectile(from_player, pos, {width, height}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov,
                   sound::blastHammer, &textures::projectile::BLAST_HAMMER, sender),
        flip(flip) {

    isDoingDamage = false;
  }
  void draw() final {
    if(spriteCounter < 110){
      DrawTextureProFastEx(resources->frames[spriteCounter % 110 / 10],
                           pos.x_ + DRAW_X - 67, pos.y_ + DRAW_Y - 95, -87, 0, flip, WHITE);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == HIT_DELAY) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct Dummy final : Projectile {
  Dummy(const Point& pos, bool from_player, float damage)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, 240, 4,
                   {DamageType::FIRE, damage}, HitType::ONE_HIT, {}, {0, 0}, 0,
                   sound::fireBurst, &textures::projectile::DUMMY, nullptr) {
    float angle = std::atan2(MOUSE_POS.y - (pos.y_ + DRAW_Y) - size.y_ / 2,
                             MOUSE_POS.x - (pos.x_ + DRAW_X) - size.x_ / 2);
    pov = angle * (180.0f / PI);
    mvmVector = GetMovementVector();
  }

  void draw() final {
    DrawTextureProFastRot(resources->frames[0], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov,
                          WHITE);
  }
};
struct EnergySphere final : Projectile {
  static constexpr float WIDTH = 30;
  static constexpr float HEIGHT = 30;
  EnergySphere(const Point& position, bool isFromPlayer, int lifespan,
               float projectileSpeed, float damageValue, HitType hitType,
               const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& statusEffects,
               const Vector2& movementDirection, const Entity* sender)
      : Projectile(isFromPlayer, position, {WIDTH, HEIGHT}, ShapeType::CIRCLE, lifespan,
                   projectileSpeed, {DamageType::ARCANE, damageValue}, hitType,
                   statusEffects, movementDirection, 0, sound::energySphere,
                   &textures::projectile::ENERGY_SPHERE, sender) {
    projectileType = ProjectileType::ENERGY_SPHERE;
    illuminated = true;
  }
  void draw() final {
    DrawTextureProFast(resources->frames[spriteCounter % 42 / 7], pos.x_ + DRAW_X - 8,
                       pos.y_ + DRAW_Y - 8, 0, WHITE);

#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
};
struct FireBall final : Projectile {
  static constexpr int width = 25;
  static constexpr int height = 25;
  FireBall(const Point& pos, bool from_player, int life_span, float speed, float damage,
           HitType hit_type,
           const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects, float pov,
           const Vector2& move, const Entity* sender)
      : Projectile(from_player, pos, {width, height}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov,
                   sound::fireBurst, &textures::projectile::FIRE_BURST, sender) {
    projectileType = ProjectileType::FIRE_BALL;
    illuminated = true;
  }
  void draw() final {
    DrawTextureProFastRotOffset(resources->frames[spriteCounter % 60 / 15],
                                pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -10, -3);
  }
};
struct PsychicScream final : Projectile {
  static constexpr int width = 25;
  static constexpr int height = 25;
  PsychicScream(const Point& pos, bool from_player, float damage, HitType hit_type)
      : Projectile(from_player, pos, {width, height}, ShapeType::CIRCLE, 42, 0,
                   {DamageType::ARCANE, damage}, hit_type, {new Stun(120)}, {0, 0}, 0,
                   sound::speak, &textures::projectile::PSYCHIC_SCREAM, nullptr) {
    isDoingDamage = false;
  }

  void draw() final {
    DrawTextureProFast(resources->frames[spriteCounter % 42 / 6], pos.x_ + DRAW_X - 65,
                       pos.y_ + DRAW_Y - 55, 0, WHITE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    Projectile::Update();
    if (lifeSpanTicks == 1) {
      isDoingDamage = true;
    }
  }
};
struct Lightning final : Projectile {
  static constexpr float WIDTH = 40;
  static constexpr float HEIGHT = 30;
  static constexpr int HIT_DELAY = 24;
  bool flip;
  Lightning(const Point& pos, bool from_player, int life_span, float speed, float damage,
            HitType hit_type,
            const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects, float pov,
            const Vector2& move, bool flip, const Entity* sender)
      : Projectile(from_player, pos, {WIDTH, HEIGHT}, ShapeType::RECT, life_span, speed,
                   {DamageType::ARCANE, damage}, hit_type, effects, move, pov,
                   sound::lightning, &textures::projectile::LIGHTNING_STRIKE, sender),
        flip(flip) {}

  void draw() final {
    if (Projectile::spriteCounter < 70) {
      DrawTextureProFastEx(resources->frames[spriteCounter % 70 / 7],
                           pos.x_ + DRAW_X - 10, pos.y_ + DRAW_Y - 53, 0, 0, flip, WHITE);
    }

#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == HIT_DELAY) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
struct FrostNova final : Projectile {
  static constexpr float WIDTH = 90;
  static constexpr float HEIGHT = 90;
  static constexpr int HIT_DELAY = 50;
  FrostNova(const Point& pos, bool from_player, int life_span, float speed, float damage,
            HitType hit_type,
            const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects, float pov,
            const Vector2& move, const Entity* sender)
      : Projectile(from_player, pos, {WIDTH, HEIGHT}, ShapeType::CIRCLE, life_span, speed,
                   {DamageType::ICE, damage}, hit_type, {new Root(300)}, move, pov,
                   sound::frostNova, &textures::projectile::FROST_NOVA, sender) {}

  void draw() final {
    if (spriteCounter > 83) return;
    DrawTextureProFastEx(resources->frames[spriteCounter % 84 / 3], pos.x_ + DRAW_X - 50,
                         pos.y_ + DRAW_Y - 50, 0, 0, false, WHITE);

#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == HIT_DELAY) {
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PROJECTILES_H_
