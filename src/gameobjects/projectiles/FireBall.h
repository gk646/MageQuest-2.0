#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_

struct FireBall : Projectile {
  FireBall(const Point& pos, const Point& size, bool from_player, float speed, float damage)
      : Projectile(from_player, pos, size, ShapeType::RECT, GetMousePosition(), 240, speed,
                   {SourceType::MAGICAL, MagicType::FIRE, damage}, ProjectileType::ONE_HIT, {}) {}
};
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
