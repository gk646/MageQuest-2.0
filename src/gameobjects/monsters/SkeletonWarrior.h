#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_

struct SkeletonWarrior final : public Monster {
  static constexpr int base_health = 10;
  SkeletonWarrior(const Point& pos, int level) : Monster(pos,EntityStats{base_health, level}){

 }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
