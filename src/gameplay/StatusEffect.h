#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

struct StatusEffect {
  StatusEffectType effect_type;
  SourceType source_type;
  int cadence;
  int duration;
  bool stack_able;
  //Texture2D texture;
  StatusEffect(StatusEffectType effect_type, SourceType source_type, int cadence, int duration,
               bool stack_able)
      : effect_type(effect_type),
        source_type(source_type),
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
    source_type = other.source_type;
    cadence = other.cadence;
    duration = other.duration;
    stack_able = other.stack_able;
    return *this;
  }
  virtual StatusEffect* clone() const = 0;
  bool operator==(const StatusEffect& other) const {
    return  effect_type == other.effect_type && source_type == other.source_type &&
              cadence == other.cadence && stack_able == other.stack_able;
  }
  void draw(int x, int y) const { DrawRectangle(x, y, 50, 50, BLUE); }
  virtual void tick(EntityStats& stats){};
  virtual void activate(EntityStats& stats) const {};
  virtual void deactivate(EntityStats& stats) const {};
  inline bool expired() const noexcept { return duration <= 0; }
  inline bool is_damage_tick() const noexcept { return duration > 0 && duration % cadence == 0; }
};

#include "effects/Burn.h"
#include "effects/Slow.h"
#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
