#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PSYCHICSCREAM_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PSYCHICSCREAM_H_
struct PsychicScream final : Projectile {
  static constexpr int width = 25;
  static constexpr int height = 25;
  PsychicScream(const Point& pos, bool from_player, float damage,
           HitType hit_type, const vector<StatusEffect*>& effects)
      : Projectile(from_player, pos, {width, height}, ShapeType::CIRCLE, 70, 0,
                   {DamageType::ARCANE, damage}, hit_type, effects, {0,0}, 0, nullptr,
                   &textures::projectile::PSYCHIC_SCREAM,nullptr) {
    does_damage = false;
  }

  void draw() final {
    DrawTextureProFast(resources->frames[sprite_counter % 56 / 8],
                       pos.x_ + DRAW_X-65, pos.y_ + DRAW_Y-55,0,WHITE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() noexcept final{
    Projectile::update();
    if(life_span == 1){
      does_damage = true;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_PSYCHICSCREAM_H_
