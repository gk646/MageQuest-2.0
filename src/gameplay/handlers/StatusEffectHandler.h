#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_

struct StatusEffectHandler {
  std::vector<StatusEffect*> currentEffects{};
  EntityStats& stats;
  explicit StatusEffectHandler(EntityStats& stats) noexcept : stats(stats) {}
  StatusEffectHandler& operator=(const StatusEffectHandler& other) {
    if (this == &other) {
      return *this;
    }

    for (auto effect : currentEffects) {
      delete effect;
    }
    currentEffects.clear();

    for (auto effect : other.currentEffects) {
      currentEffects.push_back(effect->clone());
    }

    return *this;
  }
  ~StatusEffectHandler() {
    for (auto effect : currentEffects) {
      delete effect;
    }
  }
  inline void ApplyEffects() noexcept {
    for (auto effect : currentEffects) {
      effect->ApplyEffect(stats);
    }
  }
  inline void RemoveEffects() noexcept {
    for (auto effect : currentEffects) {
      effect->RemoveEffect(stats);
    }
  }
  //Doesn't take ownership of the ptr but makes its own copy which it manages automatically
  inline void AddEffect(StatusEffect* newEffect) {
    if (!newEffect) return;
    if (!TryAddOrStackEffect(newEffect)) {

      auto new_copy = newEffect->clone();
      new_copy->ApplyEffect(stats);
      currentEffects.push_back(new_copy);
    }
  }
  inline void AddEffects(
      const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effectsArray) {
    for (const auto& newEffect : effectsArray) {
      if (!newEffect) continue;
      if (!TryAddOrStackEffect(newEffect)) {

        auto new_copy = newEffect->clone();
        new_copy->ApplyEffect(stats);
        currentEffects.push_back(new_copy);
      }
    }
  }
  void Update() noexcept {
    for (auto it = currentEffects.begin(); it != currentEffects.end();) {
      if ((*it)->duration <= 0) {
        (*it)->RemoveEffect(stats);
        delete *it;
        it = currentEffects.erase(it);
      } else {
        (*it)->TickEffect(stats);
        ++it;
      }
    }
  }
  void Draw() const noexcept {
    int buff_start = 500;
    int debuff_start = 700;
    for (const auto& effect : currentEffects) {
      effect->draw(buff_start, 100);
      buff_start += 50;
    }
  }
  //Iterates the current effects and returns the existence of the given one
  inline bool IsEffectActive(EffectType type) noexcept {
    for (const auto& e : currentEffects) {
      if (e->type == type) {
        return true;
      }
    }
    return false;
  }

 private:
  inline bool TryAddOrStackEffect(StatusEffect* newEffect) {
    for (auto& currentEffect : currentEffects) {
      if (currentEffect->type == newEffect->type) {
        currentEffect->AddStack(newEffect);
        return true;
      }
    }
    return false;
  }
};

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
