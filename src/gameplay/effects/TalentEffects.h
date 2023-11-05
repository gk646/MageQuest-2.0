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

  inline void OnSpellCast(Skill* skill) noexcept final {}
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_EFFECTS_TALENTEFFECTS_H_
