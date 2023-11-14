#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
struct Entity;
struct StatusEffectHandler {
  inline static constexpr float ROW_HEIGHT = 33;
  inline static constexpr float EFFECT_WIDTH = 33;
  std::vector<StatusEffect*> currentEffects{};
  EntityStats& stats;
  Entity* self;
  explicit StatusEffectHandler(EntityStats& stats, const Entity* ent) noexcept
      : stats(stats), self(const_cast<Entity*>(ent)) {}
  StatusEffectHandler& operator=(const StatusEffectHandler& other) {
    if (this == &other) {
      return *this;
    }

    for (auto effect : currentEffects) {
      delete effect;
    }
    currentEffects.clear();

    for (auto effect : other.currentEffects) {
      currentEffects.push_back(effect->Clone());
    }

    return *this;
  }
  ~StatusEffectHandler() {
    for (auto effect : currentEffects) {
      delete effect;
    }
  }
  //Progresses all effects by calling "TickEffect()" and deleting run out effects
  inline void Update() noexcept {
    for (auto it = currentEffects.begin(); it != currentEffects.end();) {
      if ((*it)->duration <= 0) {
        (*it)->RemoveEffect(stats, self);
        delete *it;
        it = currentEffects.erase(it);
      } else {
        (*it)->TickEffect(stats, self);
        ++it;
      }
    }
  }
  inline void ApplyEffects() noexcept {
    for (auto effect : currentEffects) {
      effect->ApplyEffect(stats, self);
    }
  }
  inline void RemoveEffects() noexcept {
    for (auto effect : currentEffects) {
      effect->RemoveEffect(stats, self);
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
  inline void RemoveEffect(EffectType type) noexcept {
    for (auto it = currentEffects.begin(); it != currentEffects.end();) {
      if ((*it)->type == type) {
        (*it)->RemoveEffect(stats, self);
        delete *it;
        currentEffects.erase(it);
        return;
      } else {
        ++it;
      }
    }
  }
  //Doesn't take ownership of the ptr but makes its own copy which it manages automatically
  inline void AddEffect(StatusEffect* newEffect) {
    if (!newEffect) return;
    if (!TryAddOrStackEffect(newEffect)) {

      auto new_copy = newEffect->Clone();
      new_copy->ApplyEffect(stats, self);
      currentEffects.push_back(new_copy);
    }
  }
  inline void AddEffects(
      const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effectsArray) {
    for (const auto& newEffect : effectsArray) {
      if (!newEffect) continue;
      if (!TryAddOrStackEffect(newEffect)) {
        auto clonedEffect = newEffect->Clone();
        clonedEffect->ApplyEffect(stats, self);
        currentEffects.push_back(clonedEffect);
      }
    }
  }

  //Effect icons
 public:
  //Draws the buffs and debuffs in the correct format for the player // left and right of the xp bar
  void DrawPlayer(float startY) const noexcept {
    float screenMiddle = SCREEN_WIDTH / 2;
    float buffX = screenMiddle - 230;
    float debuffX = screenMiddle + 230;
    float buffY = startY;
    float debuffY = startY;
    RectangleR hitBox{0, 0, 32, 32};
    StatusEffect* toolTip = nullptr;

    for (const auto& effect : currentEffects) {
      if (!effect->isDebuff) {
        effect->Draw(buffX, buffY, 32);
        if (!toolTip) {
          hitBox.x = buffX;
          hitBox.y = buffY;
          if (CheckCollisionPointRec(MOUSE_POS, hitBox)) toolTip = effect;
        }
        buffX += EFFECT_WIDTH;

        if (buffX + EFFECT_WIDTH > screenMiddle) {
          buffX = screenMiddle - 230;
          buffY += ROW_HEIGHT;
        }
      } else {
        debuffX -= EFFECT_WIDTH;
        effect->Draw(debuffX, debuffY, 32);
        if (!toolTip) {
          hitBox.x = debuffX;
          hitBox.y = debuffY;
          if (CheckCollisionPointRec(MOUSE_POS, hitBox)) toolTip = effect;
        }
        if (debuffX - EFFECT_WIDTH < screenMiddle) {
          debuffX = screenMiddle + 230;
          debuffY += ROW_HEIGHT;
        }
      }
    }

    if (toolTip) {
      toolTip->DrawToolTip(MOUSE_POS.x, MOUSE_POS.y);
    }
  }
  //Draws the buffs and debuffs in the correct format for the entities
  void DrawEntity(const Entity* entity) const noexcept {};

 private:
  inline bool TryAddOrStackEffect(const StatusEffect* newEffect) {
    for (auto& currentEffect : currentEffects) {
      if (currentEffect->type == newEffect->type) {
        currentEffect->AddStack(stats, newEffect, self);
        return true;
      }
    }
    return false;
  }
};

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECTHANDLER_H_
