#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_

struct EntityLoader {
  static void load() {
    textures::PLAYER_RESOURCE.load("player/");

    textures::SKELETON_WARRIOR.load("enemies/skeletonWarrior/");
    textures::SKELETON_SPEAR.load("enemies/skeletonSpear/");
    textures::SKELETON_SHIELD.load("enemies/skeletonShield/");
    textures::SKELETON_ARCHER.load("enemies/skeletonArcher/");
    textures::BOSS_DEATH_BRINGER.load("enemies/BOSSDeathBringer/");
    textures::BOSS_KNIGHT.load("enemies/BOSSKnight/");
    textures::BOSS_SLIME.load("enemies/BossSlime/");
    textures::GOBLIN.load("enemies/goblin/");
    textures::KNIGHT.load("enemies/Knight/");
    textures::MUSHROOM.load("enemies/mushroom/");
    textures::SNAKE.load("enemies/snake/");
    textures::WOLF.load("enemies/wolf/");

    textures::BLAST_HAMMER.load("blastHammer/");
    textures::ENERGY_SPHERE.load("EnergySphere/");
    textures::ETHER_PORTAL.load("etherPortal/");
    textures::FIRE_BURST.load("fireBurst/");
    textures::FIRE_EXPLOSION.load("fireExplosion/");
    textures::FIRE_SWORD.load("fireSword/");
    textures::FROST_NOVA.load("frostNova/");
    textures::GATE_1.load("gate1/");
    textures::ICE_LANCE.load("iceLance/");
    textures::INFERNO_RAY.load("infernoRay/");
    textures::LIGHTNING_STRIKE.load("Lightning/");
    textures::POWER_SURGE.load("powerSurge/");
    textures::PYRO_BLAST.load("pyroBlast/");
    textures::SOLAR_FLARE.load("solarFlare/");
    textures::THUNDER_SPLASH.load("thunderSplash/");
    textures::THUNDER_STRIKE.load("thunderStrike/");
    textures::VOID_ERUPTION.load("voidEruption/");
    textures::VOID_FIELD.load("voidField/");
    textures::DUMMY.load("dummy/");


    //npcs
    textures::ARIA.load("npc/aria/");
    textures::OLD_MAN.load("npc/oldman/");
    textures::realmKeeper.load("npc/realmKeeper/");
    textures::NIETZSCHE.load("npc/nietzsche/");
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
