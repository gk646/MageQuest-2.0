#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_UNIQUEEFFECTS_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_UNIQUEEFFECTS_H_

struct SpellEcho final : public UniqueEffect {
  inline void OnSpellCast(Skill* skill) noexcept final {
    if (RANGE_100(RNG_ENGINE) < 5 &&
        !PLAYER_EFFECTS.IsEffectActive(EffectType::SPELL_ECHO_CD)) {
      skill->Activate(true);
      PLAYER_EFFECTS.AddEffect(new SpellEchoCD());
    }
  }
};

struct ElementalEquilibrium final : public UniqueEffect {
  DamageType excludedType = DamageType::PHYSICAL;
  inline void OnSpellCast(Skill* skill) noexcept final {
    if (!PLAYER_EFFECTS.IsEffectActive(EffectType::ELEMENTAL_EQUILIBRIUM_BUFF) &&
        !PLAYER_EFFECTS.IsEffectActive(EffectType::ELEMENTAL_EQUILIBRIUM_CD)) {
      excludedType = skill->skillStats.dmgType;
      PLAYER_EFFECTS.AddEffect(new ElementalEquilibriumBuff(skill->skillStats.dmgType));
    }
  }
  inline void OnProjectileCreation(Projectile* prj) noexcept final {
    if (PLAYER_EFFECTS.IsEffectActive(EffectType::ELEMENTAL_EQUILIBRIUM_BUFF)) {
      auto type = prj->damageStats.dmgType;
      if (type != excludedType && Util::ArrayContains(commonSchools, type, 4)) {
        prj->damageStats.damage *= 1.3F;
        PLAYER_EFFECTS.RemoveEffect(EffectType::ELEMENTAL_EQUILIBRIUM_BUFF);
        PLAYER_EFFECTS.AddEffect(new ElementalEquilibriumCD());
      }
    }
  }
};

struct RomeoJuliet final : public UniqueEffect {
  UniqueEffect* OnAdd() noexcept final {
    std::cout<< "romeo" << std::endl;
    return this;
  }
  inline void OnProjectileCreation(Projectile* prj) noexcept final {
    if (PLAYER_STATS.health > PLAYER_STATS.GetMaxHealth() * 0.9F) {
      prj->damageStats.damage *= 1.1F;
    } else {
      prj->damageStats.damage *= 0.9F;
    }
  }
};

struct DevilsAvatar1 final : public UniqueEffect {
  UniqueEffect* OnAdd() noexcept final {
    PLAYER_STATS.effects[FIRE_DMG_P] += 0.15F;
    PLAYER_STATS.effects[HEALTH_REGEN_MULT_P] += 0.05F;
    PLAYER_STATS.effects[CRIT_DAMAGE_P] += 0.05F;
    return this;
  }
  UniqueEffect* OnRemove() noexcept final {
    PLAYER_STATS.effects[FIRE_DMG_P] -= 0.15F;
    PLAYER_STATS.effects[HEALTH_REGEN_MULT_P] -= 0.05F;
    PLAYER_STATS.effects[CRIT_DAMAGE_P] -= 0.05F;
    return this;
  }
};
struct DevilsAvatar2 final : public UniqueEffect {
  UniqueEffect* OnAdd() noexcept final {
    PLAYER_STATS.effects[CRIT_CHANCE] += 30.0F;
    PLAYER_STATS.effects[CRIT_DAMAGE_P] -=0.50F;
    return this;
  }
  UniqueEffect* OnRemove() noexcept final {
    PLAYER_STATS.effects[CRIT_CHANCE] -= 30.0F;
    PLAYER_STATS.effects[CRIT_DAMAGE_P] += 0.50F;
    return this;
  }
};
std::unordered_map<int16_t, UniqueEffect*> talentIDToEffect{
    {15, new SpellEcho()},
    {96, new ElementalEquilibrium()},
};
struct ItemSetEffects {
  UniqueEffect* effects[3];
  ItemSetEffects(UniqueEffect* one, UniqueEffect* two, UniqueEffect* three)
      : effects{one, two, three} {}
};

std::array<ItemSetEffects, (int)ItemSetNum::END> itemSetEffects{
    ItemSetEffects{new RomeoJuliet(), nullptr, nullptr},
    ItemSetEffects{new DevilsAvatar1(), new DevilsAvatar2(), nullptr}};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_UNIQUEEFFECTS_H_
