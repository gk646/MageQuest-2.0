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
  inline void ApplyEffects() const noexcept{
    for(auto effect : status_effects){
      effect->ApplyEffect(stats);
    }
  }
  inline void RemoveEffects() const noexcept{
    for(auto effect : status_effects){
      effect->RemoveEffect(stats);
    }
  }
  void AddEffects(const std::vector<StatusEffect*>& o) {
    for (const auto& new_effect : o) {
      bool is_new = true;
      for (auto& curr_effect : status_effects) {
        if (curr_effect->type == new_effect->type) {
          curr_effect->duration = new_effect->duration;
          is_new = false;
        }
      }
      if (is_new) {
        auto new_copy = new_effect->clone();
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
  void draw() const noexcept {
    int buff_start = 500;
    int debuff_start = 700;
    for (const auto& effect : status_effects) {
      effect->draw(buff_start, 100);
      buff_start += 50;
    }
  }
};

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
