#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_

struct Dummy final : Skill {
  Dummy() : Skill({}, {}) {}
  void activate() override {}
  void update() override {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_
