#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
struct EnergySphere final : Projectile {
  static constexpr int WIDTH = 30;
  static constexpr int HEIGHT = 30;
  EnergySphere(const Point& position, bool isFromPlayer, int lifespan,
               float projectileSpeed, float damageValue, HitType hitType,
               const std::vector<StatusEffect*>& statusEffects,
               const Vector2& movementDirection, const Entity* sender)
      : Projectile(isFromPlayer, position, {WIDTH, HEIGHT}, ShapeType::CIRCLE, lifespan,
                   projectileSpeed, {DamageType::ARCANE, damageValue}, hitType,
                   statusEffects, movementDirection, 0, nullptr,
                   &textures::projectile::ENERGY_SPHERE,sender) {
    projectileType = ProjectileType::ENERGY_SPHERE;
  }
  void draw() final {
    DrawTexture(resources->frames[sprite_counter % 42 / 7],pos.x_ + DRAW_X -8,
                pos.y_ + DRAW_Y - 8, WHITE);
    DrawTextureProFast(resources->frames[sprite_counter % 42 / 7], pos.x_ + DRAW_X -8,
                       pos.y_ + DRAW_Y - 8,0, WHITE);

#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_ENERGYSPHERE_H_
