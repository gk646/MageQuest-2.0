#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_

namespace EntityLoader {
static void LoadScaling() noexcept {
  auto table = Util::ReadMGI("mgi/Monsters.mgi");

  for (const auto& vec : table) {
    MonsterScaler scaler = {0};
    scaler.baseHealth = std::stof(vec[2]);
    scaler.healthPerLevel = std::stof(vec[3]);
    scaler.speed = std::stof(vec[4]);
    scaler.attackCD = (int16_t)std::stoi(vec[5]);
    scaler.attackRange = (uint8_t)std::stoi(vec[6]);
    scaler.chaseRange = (uint8_t)std::stoi(vec[7]);
    auto id = stringToMonsterID[vec[0]];
    monsterIdToScaler.insert({id, scaler});
  }
}
static void Load() {
  textures::PLAYER_RESOURCE.load("player/");

  textures::monsters::SKELETON_WARRIOR.load("enemies/skeletonWarrior/");
  textures::monsters::SKELETON_SPEAR.load("enemies/skeletonSpear/");
  textures::monsters::SKELETON_SHIELD.load("enemies/skeletonShield/");
  textures::monsters::SKELETON_ARCHER.load("enemies/skeletonArcher/");
  textures::monsters::BOSS_DEATH_BRINGER.load("enemies/BOSSDeathBringer/");
  textures::monsters::BOSS_KNIGHT.load("enemies/BOSSKnight/");
  textures::monsters::BOSS_SLIME.load("enemies/BossSlime/");
  textures::monsters::GOBLIN.load("enemies/goblin/");
  textures::monsters::KNIGHT.load("enemies/Knight/");
  textures::monsters::MUSHROOM.load("enemies/mushroom/");
  textures::monsters::SNAKE.load("enemies/snake/");
  textures::monsters::WOLF.load("enemies/wolf/");
  textures::monsters::BLOOD_HOUND.load("enemies/bloodHound/");
  textures::monsters::GHOST.load("enemies/Ghost/");

  textures::projectile::BLAST_HAMMER.load("blastHammer/");
  textures::projectile::ENERGY_SPHERE.load("EnergySphere/");
  textures::projectile::ETHER_PORTAL.load("etherPortal/");
  textures::projectile::FIRE_BURST.load("fireBurst/");
  textures::projectile::FIRE_EXPLOSION.load("fireExplosion/");
  textures::projectile::FIRE_SWORD.load("fireSword/");
  textures::projectile::FROST_NOVA.load("frostNova/");
  textures::projectile::GATE_1.load("gate1/");
  textures::projectile::ICE_LANCE.load("iceLance/");
  textures::projectile::INFERNO_RAY.load("infernoRay/");
  textures::projectile::LIGHTNING_STRIKE.load("Lightning/");
  textures::projectile::POWER_SURGE.load("powerSurge/");
  textures::projectile::PYRO_BLAST.load("pyroBlast/");
  textures::projectile::SOLAR_FLARE.load("solarFlare/");
  textures::projectile::THUNDER_SPLASH.load("thunderSplash/");
  textures::projectile::THUNDER_STRIKE.load("thunderStrike/");
  textures::projectile::VOID_ERUPTION.load("voidEruption/");
  textures::projectile::VOID_FIELD.load("voidField/");
  textures::projectile::DUMMY.load("dummy/");
  textures::projectile::PSYCHIC_SCREAM.load("PsychicScream/");
  textures::projectile::ARROW_NORMAL.load("arrowNormal/");

  textures::npc::ARIA.load("npc/aria/");
  textures::npc::OLD_MAN.load("npc/oldman/");
  textures::npc::realmKeeper.load("npc/realmKeeper/");
  textures::npc::NIETZSCHE.load("npc/nietzsche/");
  textures::npc::MARLA.load("npc/marla/");
  LoadScaling();
}

}  // namespace EntityLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
