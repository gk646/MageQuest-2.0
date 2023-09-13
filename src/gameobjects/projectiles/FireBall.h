#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_

struct FireBall final : Projectile {

  FireBall(const Point& pos, bool from_player)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, MOUSE_POS, 240,
                   3, {DamageType::FIRE, 5}, HitType::ONE_HIT, {}, FIRE_BALL) {
    resources = &textures::FIRE_BURST;
  }

  void draw() final {
    DrawTextureProFast(resources->frames[sprite_counter % 80 / 20], pos.x_ + DRAW_X,
                       pos.y_ + DRAW_Y, pov, WHITE);
  }
};
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
