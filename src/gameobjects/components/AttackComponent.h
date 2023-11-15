#ifndef MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_ATTACKCOMPONENT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_ATTACKCOMPONENT_H_

//Basic attack model
struct BaseAttack {
  const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> effects;
  const float& damage;
  float activationChance;
  const int16_t COOLDOWN_TICKS;
  int16_t currentCooldown = 0;
  int16_t animationDelay;
  int16_t currentDelay = -1;
  int8_t actionState = 0;
  bool isFar;
  BaseAttack(int8_t actionState, const float& damage, int16_t cooldown, int16_t delay,
             const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> effects,
             float activationChance, bool isFar = false)
      : actionState(actionState),
        damage(damage),
        COOLDOWN_TICKS(cooldown),
        animationDelay(delay),
        effects(effects),
        activationChance(activationChance),
        isFar(isFar) {}

 public:
  inline void Update(Monster* self) noexcept {
    if (currentCooldown > 0) {
      currentCooldown--;
    }
    if (currentDelay > 0) {
      currentDelay--;
    }
    if (currentDelay == 0) {
      currentDelay = -1;
      Execute(self);
    }
  }
  [[nodiscard]] virtual inline bool IsReady(Monster* self) noexcept {
    return currentCooldown == 0 && currentDelay == -1;
  }
  virtual void Execute(Monster* attacker) const = 0;
  virtual ~BaseAttack() {
    for (auto& ptr : effects) {
      delete ptr;
    }
  };
};
//Attacks that have an area
struct ConeAttack final : public BaseAttack {
  const Sound& sound;
  int16_t width;
  int16_t height;
  int16_t hitDelay;

 public:
  ConeAttack(int8_t actionState, const float& dmg, int16_t cd, int width, int height,
             const Sound& sound, int16_t hitDelay, int16_t animationDelay,
             const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> effects,
             float activationChance)
      : BaseAttack(actionState, dmg, cd, animationDelay, effects, activationChance),
        width((int16_t)width),
        height((int16_t)height),
        sound(sound),
        hitDelay(hitDelay) {}
  void Execute(Monster* attacker) const final;
};
//Attacks are one of the projectiles
struct ProjectileAttack final : public BaseAttack {
  int range;
  ProjectileType type;
  ProjectileAttack(int8_t actionState, const float& damage, int16_t cd, ProjectileType t,
                   int16_t delay,
                   const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> effects,
                   float activationChance, int range)
      : BaseAttack(actionState, damage, cd, delay, effects, activationChance, true),
        type(t),
        range(range) {}

 public:
  void Execute(Monster* attacker) const final;
  [[nodiscard]] virtual inline bool IsReady(Monster* self) noexcept final;
};
//Attacks were custom code is executed
struct CustomAbility final : public BaseAttack {
  int range;
  std::function<void(Monster*)> func;
  CustomAbility(int8_t actionState, int16_t cd, std::function<void(Monster*)> func,
                int16_t delay, int range, float activationChance)
      : BaseAttack(actionState, 0, cd, delay, {}, activationChance, true),
        func(func),
        range(range) {
    currentCooldown = (int)(RANGE_01(RNG_ENGINE) * cd);
  }

 public:
  void Execute(Monster* attacker) const final { func(attacker); };
  [[nodiscard]] virtual inline bool IsReady(Monster* self) noexcept final;
};
struct AttackComponent {
  std::array<BaseAttack*, 5> attacks = {0};
  Monster* self;
  int16_t currentCooldown = 0;
   int16_t GLOBAL_COOLDOWN_TICKS = 0;
  int8_t attackRangeTiles;
  int8_t chaseRangeTiles;
  int8_t registeredAttacks;
  explicit AttackComponent(Monster* self, const MonsterScaler& scaler,
                           int globalCooldown = 50)
      : attackRangeTiles(scaler.attackRangeTiles),
        chaseRangeTiles(scaler.chaseRangeTiles),
        self(self),
        registeredAttacks(0),
        GLOBAL_COOLDOWN_TICKS((int16_t)globalCooldown) {}
  ~AttackComponent() {
    for (auto& ptr : attacks) {
      delete ptr;
    }
  }
  //Called each tick // Updates the state of the component
  inline void Update() noexcept {
    for (auto& attack : attacks) {
      if (attack) {
        attack->Update(self);
      }
    }
    if (currentCooldown > 0) {
      currentCooldown--;
    }
  }

  void RegisterConeAttack(
      int8_t actionState, const float& damage, int16_t cooldown, int width, int height,
      const Sound& sound, int hitDelay, int delay = 0,
      const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> effects = {},
      float chance = 1.0F) {
    for (auto& attackSlot : attacks) {
      if (!attackSlot) {
        attackSlot = new ConeAttack(actionState, damage, cooldown, width, height, sound,
                                    (int16_t)hitDelay, (int16_t)delay, effects, chance);
        registeredAttacks++;
        return;
      }
    }
  }
  void RegisterAbility(int8_t actionState, int16_t cooldown,
                       std::function<void(Monster*)> action, int range, int delay = 0,
                       float chance = 1.0F) {
    for (auto& attackSlot : attacks) {
      if (!attackSlot) {
        attackSlot = new CustomAbility(actionState, cooldown, action, (int16_t)delay,
                                       range, chance);
        registeredAttacks++;
        return;
      }
    }
  }
  void RegisterProjectileAttack(
      int8_t actionState, const float& damage, int16_t cooldown, ProjectileType type,
      int range, int delay = 0,
      const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ> effects = {},
      float chance = 1.0F) {
    for (auto& attackSlot : attacks) {
      if (!attackSlot) {
        attackSlot = new ProjectileAttack(actionState, damage, cooldown, type,
                                          (int16_t)delay, effects, chance, range);
        registeredAttacks++;
        return;
      }
    }
  }
  //Main activation method called in the update loop
  inline void AttackClose() noexcept {
    if (currentCooldown > 0 || registeredAttacks == 0) return;
    float chance = RANGE_01(RNG_ENGINE);
    for (int i = 0; i < registeredAttacks; i++) {
      if (!attacks[i]->isFar && attacks[i]->IsReady(self) &&
          attacks[i]->activationChance >= chance) {
        StartAnimation(attacks[i]->actionState);
        attacks[i]->currentDelay = attacks[i]->animationDelay;
        attacks[i]->currentCooldown = attacks[i]->COOLDOWN_TICKS;
        currentCooldown = GLOBAL_COOLDOWN_TICKS;
        return;
      }
    }
  }
  inline void AttackFar() noexcept {
    if (currentCooldown > 0 || registeredAttacks == 0) return;
    float chance = RANGE_01(RNG_ENGINE);
    for (int i = 0; i < registeredAttacks; i++) {
      if (attacks[i]->isFar && attacks[i]->IsReady(self) &&
          attacks[i]->activationChance >= chance) {
        StartAnimation(attacks[i]->actionState);
        attacks[i]->currentDelay = attacks[i]->animationDelay;
        attacks[i]->currentCooldown = attacks[i]->COOLDOWN_TICKS;
        currentCooldown = GLOBAL_COOLDOWN_TICKS;
        return;
      }
    }
  }

  //Sets the correct action state, resets the global cooldown and creates the projectile
  inline void ExecuteAttack(int index) noexcept {
    currentCooldown = attacks[index]->COOLDOWN_TICKS;
    attacks[index]->currentCooldown = attacks[index]->COOLDOWN_TICKS;
    attacks[index]->Execute(self);
  }

 private:
  inline void StartAnimation(int8_t actionState) const noexcept;
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_ATTACKCOMPONENT_H_
