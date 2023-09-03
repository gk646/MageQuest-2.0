#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_

struct EntityLoader {
  static void load() {
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
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_ENTITYLOADER_H_
