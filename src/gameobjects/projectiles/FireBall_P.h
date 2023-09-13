#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_

struct FireBall_P final : Projectile {
  FireBall_P(const Point& pos, bool from_player)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, GetMousePosition(), 240,
                   5, {DamageType::FIRE, 5}, HitType::ONE_HIT, {}, FIRE_BALL) {}
};
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
