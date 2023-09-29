#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STUN_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STUN_H_
struct Stun final :public StatusEffect {
  explicit Stun(int duration)
      : StatusEffect(true, 0, duration, false,EffectType::STUN) {}
  [[nodiscard]] Stun* clone() const final { return new Stun(*this); }
  void ApplyEffect(EntityStats& stats) const noexcept final { stats.stunned = true; }
  void TickEffect(EntityStats& stats) final { duration--; }
  void RemoveEffect(EntityStats& stats) const noexcept final { stats.stunned = false; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_STUN_H_
