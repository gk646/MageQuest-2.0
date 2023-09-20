#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
struct EnergySphere final : Projectile {
  static constexpr int width = 15;
  static constexpr int height = 15;
  EnergySphere(const Point& pos, bool from_player, int life_span, float speed,
               float damage, HitType hit_type, const vector<StatusEffect*>& effects,const Vector2& move)
      : Projectile(from_player, pos, {width, height}, ShapeType::CIRCLE, life_span, speed,
                   {DamageType::ARCANE, damage}, hit_type, effects, move, 0, nullptr,
                   &textures::ENERGY_SPHERE) {}
  void draw() final {
    DrawTextureProFast(resources->frames[sprite_counter % 42 / 7],pos.x_ + DRAW_X -5, pos.y_ + DRAW_Y-3,0,WHITE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
