#ifndef MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
#define MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_

#include "../Projectile.h"

struct FireBall : public Projectile {
  FireBall(Point pos, Vector2 dest)
      : Projectile(pos, {25, 25}, ShapeType::CIRCLE, dest, 300, 2.5,
                   {DamageType::MAGICAL, MagicType::FIRE, ProjectileType::ONE_HIT, 5

                   }) {}
};
#endif  //MAGE_QUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_FIREBALL_H_
