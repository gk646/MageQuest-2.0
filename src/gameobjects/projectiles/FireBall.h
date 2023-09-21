#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_

struct FireBall final : Projectile {
  static constexpr int width = 25;
  static constexpr int height = 25;
  FireBall(const Point& pos, bool from_player, int life_span, float speed, float damage,
           HitType hit_type, const vector<StatusEffect*>& effects, float pov,
           const Vector2& move)
      : Projectile(from_player, pos, {width, height}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov, nullptr,
                   &textures::projectile::FIRE_BURST) {}

  void draw() final {
    DrawTextureProFastRotOffset(resources->frames[sprite_counter % 60 / 15],
                                pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov, WHITE, -10, -3);
  }
};
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
