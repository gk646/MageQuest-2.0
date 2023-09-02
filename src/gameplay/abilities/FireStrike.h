#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_

#include "../Ability.h"

struct FireStrike : Ability {

  FireStrike()
      : Ability({400, 20, 0}, {SourceType::MAGICAL, MagicType::FIRE, ProjectileType::ONE_HIT, 10}) {
  }

  void activate() final {


  }

};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
