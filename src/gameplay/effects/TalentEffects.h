#ifndef MAGEQUEST_SRC_GAMEPLAY_EFFECTS_TALENTEFFECTS_H_
#define MAGEQUEST_SRC_GAMEPLAY_EFFECTS_TALENTEFFECTS_H_

struct SpellEcho final : public TalentEffect {
  inline void OnSpellCast(Skill* skill) noexcept final {
    if (RANGE_100(RNG_ENGINE) < 5 &&
        !PLAYER_EFFECTS.IsEffectActive(EffectType::SPELL_ECHO_CD)) {
      skill->Activate(true);
      PLAYER_EFFECTS.AddEffect(new SpellEchoCD());
    }
  }
};

struct ElementalEquilibrium final : public TalentEffect {
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


std::unordered_map<int16_t, TalentEffect*> talentIDtoEffect{
    {15, new SpellEcho()},
    {96, new ElementalEquilibrium()},
};

#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_TALENTEFFECTS_H_
