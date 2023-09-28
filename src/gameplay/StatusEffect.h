#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

struct StatusEffect {
  StatusEffectType effect_type;
  uint16_t cadence;
  uint16_t duration;
  bool stack_able;
  //Texture2D texture;
  StatusEffect(StatusEffectType effect_type, int cadence, int duration, bool stack_able)
      : effect_type(effect_type),
        cadence(cadence),
        duration(duration),
        stack_able(stack_able) {}
  virtual ~StatusEffect() = default;
  StatusEffect(const StatusEffect& other) = default;
  StatusEffect& operator=(const StatusEffect& other) {
    if (this == &other) {
      return *this;
    }
    effect_type = other.effect_type;
    cadence = other.cadence;
    duration = other.duration;
    stack_able = other.stack_able;
    return *this;
  }
  [[nodiscard]] virtual StatusEffect* clone() const = 0;
  bool operator==(const StatusEffect& other) const noexcept {
    return effect_type == other.effect_type && cadence == other.cadence &&
           stack_able == other.stack_able && duration == other.duration;
  }
  static void draw(int x, int y) noexcept { DrawRectangle(x, y, 50, 50, BLUE); }
  virtual void TickEffect(EntityStats& stats) = 0;
  virtual void ApplyEffect(EntityStats& stats) const noexcept = 0;
  virtual void RemoveEffect(EntityStats& stats) const noexcept = 0;
  [[nodiscard]] inline bool is_damage_tick() const noexcept {
    return duration > 0 && duration % cadence == 0;
  }
};

#include "effects/Burn.h"
#include "effects/Slow.h"
#include "effects/Stun.h"

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
