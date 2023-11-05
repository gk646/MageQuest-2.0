#ifndef MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_ATTACKCOMPONENT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_ATTACKCOMPONENT_H_

struct BaseAttack {
  float damage;
  int16_t cooldown = 0;
  int16_t animationDelay = 0;
  int16_t currentDelay = -1;
  int8_t actionState = 0;
  BaseAttack(int8_t actionState, float damage, int16_t cooldown, int16_t delay)
      : actionState(actionState),
        damage(damage),
        cooldown(cooldown),
        animationDelay(delay) {}

 public:
  inline void Update(Monster* self) noexcept {
    if (currentDelay > 0) {
      currentDelay--;
    }
    if (currentDelay == 0) {
      currentDelay = -1;
      Execute(self);
    }
  }
  virtual void Execute(Monster* attacker) const = 0;
  virtual ~BaseAttack() = default;
};

class ConeAttack : public BaseAttack {
  const Sound& sound;
  int16_t width;
  int16_t height;
  int16_t hitDelay;

 public:
  ConeAttack(int8_t actionState, float dmg, int16_t cd, int width, int height,
             const Sound& sound, int16_t hitDelay, int16_t animationDelay)
      : BaseAttack(actionState, dmg, cd, animationDelay),
        width(width),
        height(height),
        sound(sound),
        hitDelay(hitDelay) {}
  void Execute(Monster* attacker) const final;
};
struct ProjectileAttack : public BaseAttack {
  ProjectileType type;
  ProjectileAttack(int8_t actionState, float damage, int16_t cd, ProjectileType t,
                   int16_t delay)
      : BaseAttack(actionState, damage, cd, delay), type(t) {}

 public:
  void Execute(Monster* attacker) const final;
};

struct AttackComponent {
  std::array<BaseAttack*, 5> attacks = {0};
  Monster* self;
  int16_t globalCooldown = 0;
  int8_t attackRange;
  int8_t chaseRange;
  int8_t registeredAttacks;
  explicit AttackComponent(Monster* self, const MonsterScaler& scaler)
      : attackRange(scaler.attackRange),
        chaseRange(scaler.chaseRange),
        self(self),
        registeredAttacks(0) {}
  ~AttackComponent() {
    for (auto& ptr : attacks) {
      delete ptr;
    }
  }
  inline void Update(int8_t actionState) noexcept {
    for (auto& attack : attacks) {
      if (attack) {
        attack->Update(self);
      }
    }
    if (globalCooldown > 0) {
      globalCooldown -= actionState == 0;
    }
  }
  void RegisterConeAttack(int8_t actionState, float damage, int16_t cooldown, int width,
                          int height, const Sound& sound, int hitDelay, int delay = 0) {
    for (auto& attackSlot : attacks) {
      if (!attackSlot) {
        attackSlot = new ConeAttack(actionState, damage, cooldown, width, height, sound,
                                    hitDelay, delay);
        registeredAttacks++;
        return;
      }
    }
  }
  void RegisterProjectileAttack(int8_t actionState, float damage, int16_t cooldown,
                                ProjectileType type, int delay = 0) {
    for (auto& attackSlot : attacks) {
      if (!attackSlot) {
        attackSlot = new ProjectileAttack(actionState, damage, cooldown, type, delay);
        registeredAttacks++;
        return;
      }
    }
  }
  void Attack() noexcept {
    if (globalCooldown > 0 || registeredAttacks == 0) return;
    int randomIndex = (int)std::ceil(RANGE_01(RNG_RANDOM) * 5) % registeredAttacks;
    StartAnimation(attacks[randomIndex]->actionState);
    attacks[randomIndex]->currentDelay = attacks[randomIndex]->animationDelay;
    globalCooldown = attacks[randomIndex]->cooldown;
  }

  inline void ExecuteAttack(int index) {
    attacks[index]->Execute(self);
    StartAnimation(attacks[index]->actionState);
    globalCooldown = attacks[index]->cooldown;
  }

 private:
  inline void StartAnimation(int8_t actionState) const noexcept;
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_ATTACKCOMPONENT_H_
