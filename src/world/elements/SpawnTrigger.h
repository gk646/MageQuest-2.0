#ifndef MAGEQUEST_SRC_WORLD_ELEMENTS_SPAWNTRIGGER_H_
#define MAGEQUEST_SRC_WORLD_ELEMENTS_SPAWNTRIGGER_H_
struct SpawnTrigger {
  const PointI pos;
  const PointI size;
  bool triggered;
  bool isSingular;
  TriggerSpreadType spread_type;
  uint8_t level;
  MonsterType type;
  uint8_t amount;
  //Triggers the spawn action
  inline void Trigger() noexcept;
  //Returns true if the player is close to the spawn trigger
  inline bool IsClose() const noexcept;
};
#endif  //MAGEQUEST_SRC_WORLD_ELEMENTS_SPAWNTRIGGER_H_
