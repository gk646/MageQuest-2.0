#ifndef MAGEQUEST_SRC_GAMEPLAY_TALENTEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_TALENTEFFECT_H_
struct Projectile;
struct TalentEffect {
  virtual void OnSpellCast(Skill* skill) noexcept {}
  virtual void OnSpellHit(Monster* monster) noexcept {}
  virtual void OnProjectileCreation(Projectile* prj) noexcept {}
  static inline void AddStatusEffect(Projectile* prj, StatusEffect* effect) noexcept;
};

inline static void ApplyTalentsToProjectile(Projectile* prj) noexcept {
  for (const auto& te : TALENT_EFFECTS) {
    if (te) {
      te->OnProjectileCreation(prj);
    }
  }
}
inline static void ApplyTalentsToCast(Skill* skill) noexcept {
  for (const auto& te : TALENT_EFFECTS) {
    if (te) {
      te->OnSpellCast(skill);
    }
  }
}
#endif  //MAGEQUEST_SRC_GAMEPLAY_TALENTEFFECT_H_
