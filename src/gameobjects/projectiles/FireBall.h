#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_

struct FireBall final : Projectile {
  FireBall(const Point& pos, bool from_player, float damage)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, MOUSE_POS, 240, 4,
                   {DamageType::FIRE, damage}, HitType::ONE_HIT, {}) {
    resources = &textures::FIRE_BURST;
  }
  FireBall(const Point& pos, bool from_player, float life_span, float speed, float damage,
           HitType hit_type, const vector<StatusEffect*>& effects, float pov,
           const Point& move)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, life_span, speed,
                   {DamageType::FIRE, damage}, hit_type, effects, move, pov) {
    resources = &textures::FIRE_BURST;
  }

  void draw() final {
    DrawTextureProFastRot(resources->frames[sprite_counter % 60 / 15], pos.x_ + DRAW_X,
                          pos.y_ + DRAW_Y, pov, WHITE);
  }
};
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
