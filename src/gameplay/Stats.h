#ifndef DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
#define DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
struct Entity;

struct MonsterScaler {
  std::string name{};
  float baseHealth;
  float healthPerLevel;
  float speed;
  float damage;
  int16_t attackCD;
  int8_t attackRangeTiles;
  int8_t chaseRangeTiles;
  [[nodiscard]] inline float GetMaxHealth(uint8_t level) const noexcept {
    float maxHealth = baseHealth + ((level - 1) * healthPerLevel);
    maxHealth *= DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY].healthMult;
    return maxHealth;
  }
  [[nodiscard]] inline float GetDamage(uint8_t level) const noexcept {
    float maxDamage = damage * level;
    maxDamage *= DIFFICULTY_HEALTH_MULT[GAME_DIFFICULTY].damageMult;
    return maxDamage;
  }
};

struct SkillStats {
  float manaCost = 0;
  float healthCost = 0;
  float baseDamage = 0;
  float specialVal1 = 0;
  float specialVal2 = 0;
  uint16_t castTime = 0;
  uint16_t coolDownTicks = 0;
  int16_t range = 0;
  int16_t skillLevel = 1;
  int8_t attackAnimation = 1;
  DamageType dmgType = DamageType::PHYSICAL;
  ProjectileType type = ProjectileType::LOCKED;
  [[nodiscard]] inline float GetLeveledDMG() const noexcept {
    return ((float)std::sqrt(skillLevel) * baseDamage);
  }
  [[nodiscard]] inline float GetManaCost() const noexcept {
    return manaCost * (float)skillLevel;
  }
};

struct DamageStats {
  float damage = 1;
  float critChance;
  float critDamage;
  DamageType dmgType = DamageType::FIRE;
  DamageStats(DamageType type, float damage, float critChance = 5,
              float critDamage = 0.5F)
      : damage(damage), dmgType(type), critDamage(critDamage), critChance(critChance) {}
  inline bool operator==(const DamageStats& d) const noexcept {
    return dmgType == d.dmgType && dmgType == d.dmgType && damage == d.damage;
  }
};

struct EntityStats {
  float effects[STATS_ENDING] = {0};
  float health = 10;
  float mana = 20;
  float speed = 2.6F;
  float shield = 0;
  uint8_t level = 1;
  bool stunned = false;
  DamageType lastHitType = DamageType::TRUE_DMG;
  //Player stats
  EntityStats() { SetPlayerDefaults(); };
  //Monster stats
  EntityStats(const MonsterScaler& scaler, uint8_t level)
      : level(level), speed(scaler.speed) {
    effects[MAX_HEALTH] = scaler.GetMaxHealth(level);
    health = effects[MAX_HEALTH];
    effects[WEAPON_DAMAGE] = scaler.GetDamage(level);
    effects[CRIT_CHANCE] = 5;
    effects[CRIT_DAMAGE_P] = 0.5F;
    effects[HEALTH_REGEN] = effects[MAX_HEALTH] / 7200;
    effects[DODGE_CHANCE] = 1;
  }
  inline void Update() noexcept {
    float max_mana_value = GetMaxMana();
    if (mana < max_mana_value) {
      mana = std::min(mana + GetManaRegen() / 60, max_mana_value);
    } else {
      mana = max_mana_value;
    }

    float max_health_value = GetMaxHealth();
    if (health < max_health_value) {
      health = std::min(health + GetHealthRegen() / 60, max_health_value);
    } else {
      health = max_health_value;
    }

    float max_shield = effects[MAX_SHIELD];
    if (shield < max_shield && mana > 0) {
      shield += effects[MANA_REGEN] / 60;
      mana -= effects[MANA_REGEN] / 60;
    } else if (shield > max_shield) {
      shield = std::max(shield - effects[MANA_REGEN] / 60, max_shield);
    }
  }
  inline void ApplySkillCosts(const SkillStats& stats) noexcept {
    mana -= stats.GetManaCost() * (1 - effects[MANA_COST_REDUCTION_P]);
  }
  inline void EquipItem(const float* effectArr) noexcept {
    for (int i = 0; i < STATS_ENDING; i++) {
      effects[i] += effectArr[i];
    }
    CheckForItemSets();
    ReCalculatePlayerStats();
  }
  inline void UnEquipItem(const float* effect_arr) noexcept;
  inline void RefillStats() noexcept {
    mana = GetMaxMana();
    health = GetMaxHealth();
    shield = effects[MAX_SHIELD];
  }
  inline void SpendAttributePoint(uint8_t i) noexcept {
    effects[i]++;
    ReCalculatePlayerStats();
  }
  inline void ReCalculatePlayerStats() noexcept;
  inline float TakeDamage(const DamageStats& dmgStats, const Entity* ent);
  static inline void CheckForItemSets() noexcept;
  inline void SetPlayerDefaults() noexcept {
    effects[CRIT_CHANCE] = 0;
    effects[CRIT_DAMAGE_P] = 0.5F;
    effects[HEALTH_REGEN] = 0.2F;
    effects[MANA_REGEN] = 1;
    effects[MAX_HEALTH] = 10;
    effects[MAX_MANA] = 20;
    effects[WEAPON_DAMAGE] = 1;
    effects[DODGE_CHANCE] = 0;
    effects[BAG_SLOTS] = 9;
  }
  //Setters
 public:
  inline void AddHealth(float val) noexcept {
    health = std::min(health + val, GetMaxHealth());
  }
  //Getters
 public:
  [[nodiscard]] inline float GetAbilityDmg(float damage,
                                           DamageType dmgType) const noexcept {

    damage *= effects[WEAPON_DAMAGE];
    switch (dmgType) {
      case DamageType::FIRE:
        return damage * (1 + effects[FIRE_DMG_P]);
      case DamageType::POISON:
        return damage * (1 + effects[POISON_DMG_P]);
      case DamageType::ICE:
        return damage * (1 + effects[ICE_DMG_P]);
      case DamageType::ARCANE:
        return damage * (1 + effects[ARCANE_DMG_P]);
      case DamageType::DARK:
        return damage * (1 + effects[DARK_DMG_P]);
      case DamageType::PHYSICAL:
      case DamageType::TRUE_DMG:
        return damage;
    }
  }
  [[nodiscard]] inline float GetTotalCD(const SkillStats& stats) const noexcept {
    return (float)stats.coolDownTicks * (1 - effects[CDR_P]);
  }
  [[nodiscard]] inline float GetMaxHealth() const noexcept {
    return effects[MAX_HEALTH] * (1 + effects[HEALTH_MULT_P]);
  }
  [[nodiscard]] inline float GetMaxMana() const noexcept {
    return effects[MAX_MANA] * (1 + effects[MANA_MULT_P]);
  }
  [[nodiscard]] inline float GetSpeed() const noexcept {
    return speed * (1 + effects[SPEED_MULT_P]);
  }
  [[nodiscard]] inline float GetManaRegen() const noexcept {
    return effects[MANA_REGEN] * (1 + effects[MANA_REGEN_MULT_P]);
  }
  [[nodiscard]] inline float GetHealthRegen() const noexcept {
    return effects[HEALTH_REGEN] * (1 + effects[HEALTH_REGEN_MULT_P]);
  }
  [[nodiscard]] inline float GetArmour() const noexcept {
    return effects[ARMOUR] * (1 + effects[ARMOUR_MULT_P]);
  }
  [[nodiscard]] inline float GetBagSlots() const noexcept { return effects[BAG_SLOTS]; }
  [[nodiscard]] inline bool IsSkillUsable(const SkillStats& stats,
                                          float ticks_done) const noexcept {
    return !stunned && ticks_done >= GetTotalCD(stats) &&
           mana >= stats.GetManaCost() * (1 - effects[MANA_COST_REDUCTION_P]);
  }

 private:
  [[nodiscard]] inline static bool RollCriticalHit(const DamageStats& stats) noexcept {
    return RANGE_100_FLOAT(RNG_ENGINE) < stats.critChance;
  }
};

inline EntityStats PLAYER_STATS;

struct PlayerStats {
  float experience = 0;
  int32_t prevLevelReq = 0;
  int32_t nextLevelReq = 0;
  int32_t coins = 0;
  int16_t spentAttributePoints[9] = {0};
  int16_t attributePointsToSpend = 0;
  int16_t spentTalentPoints = 0;
  int16_t talentPointsToSpend = 25;
  PlayerStats() { UpdateRequirements(1); }
  //Getters
 public:
  [[nodiscard]] inline bool IsDefaultValue(Stat stat) const noexcept {
    return spentAttributePoints[stat] == (int)PLAYER_STATS.effects[stat];
  }
  [[nodiscard]] inline bool HasAttributePointsToSpend() const noexcept {
    return attributePointsToSpend > 0;
  }
  [[nodiscard]] inline bool HasTalentPointsToSpend() const noexcept {
    return talentPointsToSpend > 0;
  }

 public:
  inline void AddCoins(int32_t val) noexcept {
    coins += (int)std::round((float)val * (1.0F + PLAYER_STATS.effects[GOLD_PICKUP_P]));
  }
  inline bool SpendAttributePoint(uint8_t i) noexcept {
    if (attributePointsToSpend > 0) {
      spentAttributePoints[i]++;
      attributePointsToSpend--;
      return true;
    }
    return false;
  }
  inline bool SpendTalentPoint() noexcept {
    if (talentPointsToSpend > 0) {
      spentTalentPoints++;
      talentPointsToSpend--;
      return true;
    }
    return false;
  }
  inline void AddPlayerExperience(float val) noexcept {
    experience += val * (1 + PLAYER_STATS.effects[XP_MODIFIER_P]);
    CheckForLevelUp();
  }
  inline void LoadExperience(int newExperience) noexcept {
    experience = static_cast<float>(newExperience);
    CheckForLevelUp();
  }
  //Check if the player has enough experience to level up
  inline void CheckForLevelUp() noexcept {
    while ((int)experience - prevLevelReq >= nextLevelReq) {
      LevelUP();
    }
  }
  // Called on a level up event
  inline void LevelUP() noexcept {
    PLAYER_STATS.level++;
    UpdateRequirements(PLAYER_STATS.level);
    AddLevelUPPoints();
    PLAYER_STATS.ReCalculatePlayerStats();
    IncrementSkillLevels();
    // TODO: animation
  }
  static inline int GetXPRequiredForLevel(int level) noexcept {
    if (level == 1) return 0;
    return static_cast<int>((5 * level) * std::sqrt(level) * std::pow(1.05, level));
  }

 private:
  inline void AddLevelUPPoints() noexcept {
    attributePointsToSpend += 3;
    talentPointsToSpend++;
  }
  inline void UpdateRequirements(int level) noexcept {
    prevLevelReq += nextLevelReq;
    nextLevelReq = GetXPRequiredForLevel(level + 1);
  }
  static inline void IncrementSkillLevels() noexcept;
};

inline static PlayerStats PLAYER_SECOND_STATS{};
inline static std::unordered_map<MonsterType, MonsterScaler> monsterIdToScaler{};
#endif  //DUNGEON_MASTER_SRC_ENTITIES_STATS_STATS_H_
