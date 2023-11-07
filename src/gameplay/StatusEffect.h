#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

struct StatusEffect {
  int16_t cadence;
  int16_t duration;
  int16_t stacks = 0;
  EffectType type;
  bool isDebuff;
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
  inline void Draw(float x, float y, float size) const noexcept {
    DrawTextureScaled(effectToInfo[type].icon, {x, y, size, size}, 0, false, 0, WHITE);
  }
  virtual inline void TickEffect(EntityStats& stats) = 0;
  virtual inline void ApplyEffect(EntityStats& stats) noexcept = 0;
  virtual inline void RemoveEffect(EntityStats& stats) noexcept = 0;
  virtual inline void AddStack(StatusEffect* other) noexcept = 0;
  [[nodiscard]] inline bool IsDamageTick() const noexcept {
    return duration > 0 && duration % cadence == 0;
  }
};

#include "effects/StatusEffects.h"

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
