#ifndef MAGEQUEST_SRC_GAMEPLAY_TALENTEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_TALENTEFFECT_H_
struct Projectile;
struct TalentEffect {
  virtual void OnSpellCast(Skill* skill) noexcept {}
  virtual void OnSpellHit(Monster* monster) noexcept {}
  virtual void OnProjectileCreation(Projectile* prj) noexcept {}
  virtual void OnMonsterDeath(Monster* monster) noexcept {}
  virtual void OnPlayerHit(Projectile* prj) noexcept {}
};
inline static void ApplyTalentsToProjectile(Projectile* prj) noexcept {
  for (const auto& te : TALENT_EFFECTS) {
    te->OnProjectileCreation(prj);
  }
}
inline static void ApplyTalentsToCast(Skill* skill) noexcept {
  for (const auto& te : TALENT_EFFECTS) {
    te->OnSpellCast(skill);
  }
}
inline static void ApplyTalentsToMonsterDeath(Monster* monster) noexcept {
  for (const auto& te : TALENT_EFFECTS) {
    te->OnMonsterDeath(monster);
  }
}
inline static void ApplyTalentsToPlayerHit(Projectile* prj) noexcept {
  for (const auto& te : TALENT_EFFECTS) {
    te->OnPlayerHit(prj);
  }
}
#endif  //MAGEQUEST_SRC_GAMEPLAY_TALENTEFFECT_H_
