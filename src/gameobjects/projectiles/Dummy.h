#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_DUMMY_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_DUMMY_H_
struct Dummy final : Projectile {
  Dummy(const Point& pos, bool from_player, float damage)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, MOUSE_POS, 240, 4,
                   {DamageType::FIRE, damage}, HitType::ONE_HIT, {}) {
    resources = &textures::projectile::DUMMY;
  }
  Dummy(const Point& pos, bool from_player, float life_span, float speed, float damage,
           HitType hit_type, const vector<StatusEffect*>& effects, float pov,
           const Vector2 & move)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov,nullptr,&textures::projectile::DUMMY) {
  }

  void draw() final {
    DrawTextureProFastRot(resources->frames[0], pos.x_ + DRAW_X,
                          pos.y_ + DRAW_Y, pov, WHITE);
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_DUMMY_H_
