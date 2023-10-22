#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_

struct StatusEffectHandler {
  std::vector<StatusEffect*> status_effects{};
  EntityStats& stats;
  explicit StatusEffectHandler(EntityStats& stats) noexcept : stats(stats) {}
  ~StatusEffectHandler(){
    for(auto effect : status_effects){
      delete effect;
    }
  }
  inline void ApplyEffects()  noexcept{
    for(auto effect : status_effects){
      effect->ApplyEffect(stats);
    }
  }
  inline void RemoveEffects()  noexcept{
    for(auto effect : status_effects){
      effect->RemoveEffect(stats);
    }
  }
  void AddEffects(const std::array<StatusEffect*,MAX_STATUS_EFFECTS_PRJ>& o) {
    for (const auto& newEffect : o) {
      if(!newEffect) continue;
      bool is_new = true;
      for (auto& currentEffect : status_effects) {
        if (currentEffect->type == newEffect->type) {
          currentEffect->duration = newEffect->duration;
          is_new = false;
        }
      }
      if (is_new) {
        auto new_copy = newEffect->clone();
        new_copy->ApplyEffect(stats);
        status_effects.push_back(new_copy);
      }
    }
  }
  void Update() noexcept {
    for (auto it = status_effects.begin(); it != status_effects.end();) {
      if ((*it)->duration <= 0) {
        (*it)->RemoveEffect(stats);
        delete *it;
        it = status_effects.erase(it);
      } else {
        (*it)->TickEffect(stats);
        ++it;
      }
    }
  }
  void Draw() const noexcept {
    int buff_start = 500;
    int debuff_start = 700;
    for (const auto& effect : status_effects) {
      effect->draw(buff_start, 100);
      buff_start += 50;
    }
  }
};

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
