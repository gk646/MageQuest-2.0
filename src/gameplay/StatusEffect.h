#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

struct StatusEffect {
  int16_t cadence;
  int16_t duration;
  int16_t stacks = 0;
  EffectType type;
  bool isDebuff;
  //Texture2D texture;
  StatusEffect(bool isDebuff, int cadence, int duration, EffectType type)
      : isDebuff(isDebuff),
        cadence((int16_t)cadence),
        duration((int16_t)duration),
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
    return *this;
  }
  [[nodiscard]] virtual StatusEffect* clone() const = 0;
  static void draw(int x, int y) noexcept { DrawRectangle(x, y, 50, 50, BLUE); }
  virtual void TickEffect(EntityStats& stats) = 0;
  virtual void ApplyEffect(EntityStats& stats) noexcept = 0;
  virtual void RemoveEffect(EntityStats& stats) noexcept = 0;
  virtual void AddStack(StatusEffect* other) noexcept = 0;
  [[nodiscard]] inline bool IsDamageTick() const noexcept {
    return duration > 0 && duration % cadence == 0;
  }
};

#include "effects/StatusEffects.h"

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
