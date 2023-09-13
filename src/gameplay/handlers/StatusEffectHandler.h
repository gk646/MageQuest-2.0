#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_

struct StatusEffectHandler {
  EntityStats& stats;
  bool effects_changed = false;
  std::vector<StatusEffect*> status_effects{};
  explicit StatusEffectHandler(EntityStats& stats) noexcept : stats(stats) {}
  ~StatusEffectHandler(){
    for(auto effect : status_effects){
      delete effect;
    }
  }
  void add_effects(const std::vector<StatusEffect*>& o) {
    for (const auto& new_effect : o) {
      bool is_new = true;
      for (auto& curr_effect : status_effects) {
        if (*curr_effect == *new_effect) {
          curr_effect->duration = new_effect->duration;
          is_new = false;
        }
      }
      if (is_new) {
        auto new_copy = new_effect->clone();
        new_copy->activate(stats);
        status_effects.push_back(new_copy);
        effects_changed = true;
      }
    }
  }
  void update() noexcept {
    if (effects_changed) {
      for (const auto& effect : status_effects) {
        effect->activate(stats);
      }
      effects_changed = false;
    }

    for (auto it = status_effects.begin(); it != status_effects.end();) {
      if ((*it)->expired()) {
        (*it)->deactivate(stats);
        delete *it;
        it = status_effects.erase(it);
        effects_changed = true;
      } else {
        (*it)->tick(stats);
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
