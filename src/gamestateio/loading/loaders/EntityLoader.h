#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_

namespace EntityLoader {
//Loads the enemies scaling and behaviour from file
static void LoadScaling() noexcept {
  auto table = Util::ReadMGI("mgi/Monsters.mgi");

  for (const auto& vec : table) {
    MonsterScaler scaler{};
    scaler.name = vec[1];
    scaler.baseHealth = std::stof(vec[2]);
    scaler.healthPerLevel = std::stof(vec[3]);
    scaler.speed = std::stof(vec[4]);
    scaler.attackCD = (int16_t)std::stoi(vec[5]);
    scaler.attackRangeTiles = (int8_t)std::stoi(vec[6]);
    scaler.chaseRangeTiles = (int8_t)std::stoi(vec[7]);
    scaler.damage = std::stof(vec[9]);
    auto id = stringToMonsterID[vec[0]];
    monsterIdToScaler.insert({id, scaler});
  }
}
//Loads self resources
static void Load() {
  textures::PLAYER_RESOURCE.Load("player/");

  textures::monsters::SKELETON_WARRIOR.Load("enemies/skeletonWarrior/");
  textures::monsters::SKELETON_SPEAR.Load("enemies/skeletonSpear/");
  textures::monsters::SKELETON_SHIELD.LoadFromSpriteSheet("enemies/skeletonShield/", 150,
                                                          150);
  textures::monsters::SKELETON_ARCHER.Load("enemies/skeletonArcher/");
  textures::monsters::BOSS_DEATH_BRINGER.Load("enemies/BOSSDeathBringer/");
  textures::monsters::BOSS_STONE_KNIGHT.LoadFromSpriteSheet("enemies/BOSSKnight/",192,96);
  textures::monsters::BOSS_SLIME.Load("enemies/BossSlime/");
  textures::monsters::GOBLIN.LoadFromSpriteSheet("enemies/goblin/", 150, 150);
  textures::monsters::KNIGHT.Load("enemies/Knight/");
  textures::monsters::MUSHROOM.LoadFromSpriteSheet("enemies/mushroom/", 150, 150);
  textures::monsters::SNAKE.Load("enemies/snake/");
  textures::monsters::RAT.LoadFromSpriteSheet("enemies/rat/", 32, 32);
  textures::monsters::WOLF.Load("enemies/wolf/");
  textures::monsters::BLOOD_HOUND.Load("enemies/bloodHound/");
  textures::monsters::GHOST.Load("enemies/Ghost/");
  textures::monsters::SKULL_WOLF.LoadFromSpriteSheet("enemies/skullWolf/", 64, 48);
  textures::monsters::FLYING_EYE.LoadFromSpriteSheet("enemies/flyingEye/", 150, 150);
  textures::monsters::BOSS_STONE_GOLEM.LoadFromSpriteSheet("enemies/BOSSStoneGolem/", 200,
                                                           164);

  textures::projectile::BLAST_HAMMER.Load("blastHammer/");
  textures::projectile::ENERGY_SPHERE.Load("EnergySphere/");
  textures::projectile::ETHER_PORTAL.Load("etherPortal/");
  textures::projectile::FIRE_BURST.Load("fireBurst/");
  textures::projectile::FIRE_BALL.LoadSpriteSheet("projectiles/fireBall/0.png", 64, 64,
                                                  48, 0);
  textures::projectile::FIRE_EXPLOSION.Load("fireExplosion/");
  textures::projectile::FIRE_SWORD.Load("fireSword/");
  textures::projectile::FROST_NOVA.Load("frostNova/");
  textures::projectile::GATE_1.Load("gate1/");
  textures::projectile::ICE_LANCE.LoadSpriteSheet("projectiles/iceLance/0.png", 48, 32);
  textures::projectile::GLACIAL_BURST.LoadSpriteSheet("projectiles/glacialBurst/0.png",
                                                      64, 64);
  textures::projectile::INFERNO_RAY.Load("infernoRay/");
  textures::projectile::LIGHTNING_STRIKE.Load("Lightning/");
  textures::projectile::POWER_SURGE.Load("powerSurge/");
  textures::projectile::PYRO_BLAST.Load("pyroBlast/");
  textures::projectile::SOLAR_FLARE.Load("solarFlare/");
  textures::projectile::THUNDER_SPLASH.Load("thunderSplash/");
  textures::projectile::THUNDER_STRIKE.Load("thunderStrike/");
  textures::projectile::VOID_ERUPTION.Load("voidEruption/");
  textures::projectile::BOMB.LoadSpriteSheet("projectiles/bomb/0.png",100,100);
  textures::projectile::VOID_FIELD.LoadSpriteSheet("projectiles/voidField/0.png", 124,
                                                   124);
  textures::projectile::DUMMY.Load("dummy/");
  textures::projectile::PSYCHIC_SCREAM.Load("PsychicScream/");
  textures::projectile::ARROW_NORMAL.Load("arrowNormal/");
  textures::projectile::ARCANE_BOLT.Load("arcaneBolt/");
  textures::projectile::SPORE_SHOT.LoadSpriteSheet("projectiles/sporeShot/0.png", 50, 50);
  textures::projectile::SWORD_SPIN.LoadSpriteSheet("projectiles/swordSpin/0.png", 92, 102);

  textures::npc::ARIA.Load("npc/aria/");
  textures::npc::OLD_MAN.Load("npc/oldman/");
  textures::npc::realmKeeper.Load("npc/realmKeeper/");
  textures::npc::NIETZSCHE.Load("npc/nietzsche/");
  textures::npc::MARLA.Load("npc/marla/");
  textures::npc::SATIRO.Load("npc/satiro/");

  LoadImagesIntoVector("Entities/container/basicChest/", textures::container::basicChest);
  LoadImagesIntoVector("Entities/container/normalChest/",
                       textures::container::normalChest);
  LoadImagesIntoVector("Entities/container/epicChest/", textures::container::epicChest);

  LoadScaling();
}

}  // namespace EntityLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
