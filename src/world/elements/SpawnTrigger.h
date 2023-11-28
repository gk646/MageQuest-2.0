#ifndef MAGEQUEST_SRC_WORLD_ELEMENTS_SPAWNTRIGGER_H_
#define MAGEQUEST_SRC_WORLD_ELEMENTS_SPAWNTRIGGER_H_

struct SpawnTrigger {
  const PointI pos;
  const PointI size;
  bool triggered = false;
  bool isSingular;
  TriggerSpreadType spread_type;
  uint8_t level = 0;
  MonsterType type;
  uint8_t amount = 0;
  //Triggers the spawn action
  inline void Trigger() noexcept;
  //Returns true if the player is close to the spawn trigger
  [[nodiscard]] inline bool IsClose() const noexcept;
};

#endif  //MAGEQUEST_SRC_WORLD_ELEMENTS_SPAWNTRIGGER_H_
