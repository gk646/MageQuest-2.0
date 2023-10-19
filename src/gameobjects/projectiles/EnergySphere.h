#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
struct EnergySphere final : Projectile {
  static constexpr float WIDTH = 30;
  static constexpr float HEIGHT = 30;
  EnergySphere(const Point& position, bool isFromPlayer, int lifespan,
               float projectileSpeed, float damageValue, HitType hitType,
               const  std::array<StatusEffect*,MAX_STATUS_EFFECTS_PRJ>& statusEffects,
               const Vector2& movementDirection, const Entity* sender)
      : Projectile(isFromPlayer, position, {WIDTH, HEIGHT}, ShapeType::CIRCLE, lifespan,
                   projectileSpeed, {DamageType::ARCANE, damageValue}, hitType,
                   statusEffects, movementDirection, 0, &sound::energySphere,
                   &textures::projectile::ENERGY_SPHERE, sender) {
    projectileType = ProjectileType::ENERGY_SPHERE;
    illuminated = true;
  }
  void draw() final {
    DrawTextureProFast(resources->frames[spriteCounter % 42 / 7], pos.x_ + DRAW_X - 8,
                       pos.y_ + DRAW_Y - 8, 0, WHITE);

#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
