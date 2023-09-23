#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_LIGHTNING_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_LIGHTNING_H_
struct Lightning final : Projectile {
  static constexpr int width = 25;
  static constexpr int height = 25;
  Lightning(const Point& pos, bool from_player, int life_span, float speed, float damage,
           HitType hit_type, const vector<StatusEffect*>& effects, float pov,
           const Vector2& move,const Entity* sender)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov,nullptr, nullptr,sender) {
  }

  void draw() final {
    DrawTextureProFastRotOffset(resources->frames[sprite_counter % 60 / 15], pos.x_ + DRAW_X,
                                pos.y_ + DRAW_Y, pov, WHITE,-10,-3);

  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_LIGHTNING_H_
