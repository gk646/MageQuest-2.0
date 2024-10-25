#ifndef MAGEQUEST_SRC_GAMEPLAY_TALENTS_TALENT_H_
#define MAGEQUEST_SRC_GAMEPLAY_TALENTS_TALENT_H_

struct Talent {
  float effects[STATS_ENDING] = {0};
  std::string description;
  std::string name;
  Texture icon;
  UniqueEffect* talentEffect = nullptr;
};

#endif  //MAGEQUEST_SRC_GAMEPLAY_TALENTS_TALENT_H_
