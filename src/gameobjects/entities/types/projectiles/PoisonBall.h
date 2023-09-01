#ifndef DUNGEONM_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_POISONBALL_H_
#define DUNGEONM_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_POISONBALL_H_

#include "../Projectile.h"

struct PoisonBall : public Projectile {
  PoisonBall(Point pos, Vector2 dest)
      : Projectile(pos, {15, 15}, ShapeType::CIRCLE, dest, 120, 3.5,
                   {DamageType::MAGICAL, MagicType::POISON,  ProjectileType::ONE_HIT,1

                   }) {}
};
#endif  //DUNGEONM_SRC_GAMEOBJECTS_ENTITIES_TYPES_PROJECTILES_POISONBALL_H_
