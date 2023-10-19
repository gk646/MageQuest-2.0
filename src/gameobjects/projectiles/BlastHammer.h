#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_BLASTHAMMER_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_BLASTHAMMER_H_
struct BlastHammer final : Projectile {
  static constexpr int width = 50;
  static constexpr int height = 50;
  static constexpr int hit_delay = 50;
  bool flip;
  BlastHammer(const Point& pos, bool from_player, int life_span, float speed,
              float damage, HitType hit_type, const std::array<StatusEffect*,3>& effects,
              float pov, const Vector2& move, bool flip, const Entity* sender)
      : Projectile(from_player, pos, {width, height}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov, &sound::blastHammer,
                   &textures::projectile::BLAST_HAMMER,sender),
        flip(flip) {

    isDoingDamage = false;
  }
  void draw() final {
    DrawTextureProFastEx(resources->frames[spriteCounter % 110 / 10],
                         pos.x_ + DRAW_X - 70, pos.y_ + DRAW_Y - 95, -70, 0, flip, WHITE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    Projectile::Update();
    if (spriteCounter == hit_delay) {
      PlaySoundR(sound::blastHammer);
      isDoingDamage = true;
    } else {
      isDoingDamage = false;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_BLASTHAMMER_H_
