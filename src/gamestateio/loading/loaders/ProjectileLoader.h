#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_PROJECTILELOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_PROJECTILELOADER_H_
namespace ProjectileLoader {
inline static ProjectileResources* GetProjectileRes(ProjectileType type) noexcept {
  switch (type) {
    case POISON_BALL:
      break;
    case FIRE_STRIKE:
      return &textures::projectile::FIRE_BURST;
    case FIRE_STRIKE_II:
      return &textures::projectile::FIRE_BURST;
    case FIRE_BALL:
      return &textures::projectile::FIRE_BALL;
    case ARROW_NORMAL:
      return &textures::projectile::ARROW_NORMAL;
    case BLAST_HAMMER:
      return &textures::projectile::BLAST_HAMMER;
    case ENERGY_SPHERE:
      return &textures::projectile::ENERGY_SPHERE;
    case FIRE_SWORD:
      return &textures::projectile::FIRE_SWORD;
    case FROST_NOVA:
      return &textures::projectile::FROST_NOVA;
    case ICE_LANCE:
      return &textures::projectile::ICE_LANCE;
    case INFERNO_RAY:
      return &textures::projectile::INFERNO_RAY;
    case LIGHTNING:
      return &textures::projectile::LIGHTNING_STRIKE;
    case PYRO_BLAST:
      return &textures::projectile::PYRO_BLAST;
    case SOLAR_FLARE:
      return &textures::projectile::SOLAR_FLARE;
    case THUNDER_SPLASH:
      return &textures::projectile::THUNDER_SPLASH;
    case THUNDER_STRIKE:
      return &textures::projectile::THUNDER_STRIKE;
    case VOID_ERUPTION:
      return &textures::projectile::VOID_ERUPTION;
    case VOID_FIELD:
      return &textures::projectile::VOID_FIELD;
    case ARCANE_BOLT:
      return &textures::projectile::ARCANE_BOLT;
    case PROJECTILE_END:
      break;
  }
  return nullptr;
}
static void Load() noexcept {
  auto ret = Util::ReadMGI("mgi/Projectiles.mgi");
  for (const auto& vec : ret) {
    auto& info = typeToInfo[stringToProjectile[vec[0]]];
    if (vec[1] == "one_tick") {
      info.hitType = HitType::ONE_TICK;
    } else if (vec[1] == "cont") {
      info.hitType = HitType::CONTINUOUS;
    } else {
      info.hitType = HitType::ONE_HIT;
    }
    info.lifeSpan = (int16_t)std::stoi(vec[2]);
    info.speed = std::stof(vec[3]);
    info.hitBoxShape = vec[4] == "rect" ? ShapeType::RECT : ShapeType::CIRCLE;
    info.size = {(int16_t)std::stoi(vec[5]), (int16_t)std::stoi(vec[6])};
    info.val1 = std::stof(vec[7]);
    info.illuminated = std::stoi(vec[8]) == 1;
    info.res = GetProjectileRes(stringToProjectile[vec[0]]);
  }
}
}  // namespace ProjectileLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_PROJECTILELOADER_H_
