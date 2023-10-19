#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PSYCHICSCREAM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PSYCHICSCREAM_H_
struct PsychicScream final : Projectile {
  static constexpr int width = 25;
  static constexpr int height = 25;
  PsychicScream(const Point& pos, bool from_player, float damage,
           HitType hit_type)
      : Projectile(from_player, pos, {width, height}, ShapeType::CIRCLE, 42, 0,
                   {DamageType::ARCANE, damage}, hit_type, {new Stun(120)}, {0,0}, 0, nullptr,
                   &textures::projectile::PSYCHIC_SCREAM,nullptr) {
    isDoingDamage = false;
  }

  void draw() final {
    DrawTextureProFast(resources->frames[spriteCounter % 42 / 6],
                       pos.x_ + DRAW_X-65, pos.y_ + DRAW_Y-55,0,WHITE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final{
    Projectile::Update();
    if(lifeSpanTicks == 1){
      isDoingDamage = true;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PSYCHICSCREAM_H_
