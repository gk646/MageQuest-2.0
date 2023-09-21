#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_

struct EntityLoader {
  static void load() {
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


    //npcs
    textures::npc::ARIA.load("npc/aria/");
    textures::npc::OLD_MAN.load("npc/oldman/");
    textures::npc::realmKeeper.load("npc/realmKeeper/");
    textures::npc::NIETZSCHE.load("npc/nietzsche/");
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
