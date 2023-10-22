#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

struct StatusEffect {
  uint16_t cadence;
  uint16_t duration;
  EffectType type;
  bool isDebuff;
  bool stack_able;
  //Texture2D texture;
  StatusEffect(bool isDebuff, int cadence, int duration, bool stack_able, EffectType type)
      : isDebuff(isDebuff),
        cadence((uint16_t)cadence),
        duration((uint16_t)duration),
        stack_able(stack_able),
        type(type) {}
  virtual ~StatusEffect() = default;
  StatusEffect(const StatusEffect& other) = default;
  StatusEffect& operator=(const StatusEffect& other) {
    if (this == &other) {
      return *this;
    }
    isDebuff = other.isDebuff;
    cadence = other.cadence;
    duration = other.duration;
    stack_able = other.stack_able;
    return *this;
  }
  [[nodiscard]] virtual StatusEffect* clone() const = 0;
  static void draw(int x, int y) noexcept { DrawRectangle(x, y, 50, 50, BLUE); }
  virtual void TickEffect(EntityStats& stats) = 0;
  virtual void ApplyEffect(EntityStats& stats)  noexcept = 0;
  virtual void RemoveEffect(EntityStats& stats)  noexcept = 0;
  [[nodiscard]] inline bool is_damage_tick() const noexcept {
    return duration > 0 && duration % cadence == 0;
  }
};

#include "effects/Effects.h"

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
