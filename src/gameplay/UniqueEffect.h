#ifndef MAGEQUEST_SRC_GAMEPLAY_UNIQUEEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_UNIQUEEFFECT_H_
struct Projectile;
struct UniqueEffect {
  inline virtual void OnSpellCast(Skill* skill) noexcept {}
  inline virtual void OnSpellHit(Monster* monster) noexcept {}
  inline virtual void OnProjectileCreation(Projectile* prj) noexcept {}
  inline virtual void OnMonsterDeath(Monster* monster) noexcept {}
  inline virtual void OnPlayerHit(Projectile* prj) noexcept {}
  inline virtual UniqueEffect* OnAdd() noexcept { return this; }
  inline virtual UniqueEffect* OnRemove() noexcept { return this; }
  inline static void RemoveUniqueEffects() noexcept{
    for(auto& effect : UNIQUE_EFFECTS){
      effect->OnRemove();
    }
  }
  inline static void ApplyUniqueEffects() noexcept{
    for(auto& effect : UNIQUE_EFFECTS){
      effect->OnAdd();
    }
  }
};

inline static void ApplyTalentsToProjectile(Projectile* prj) noexcept {
  for (const auto& te : UNIQUE_EFFECTS) {
    te->OnProjectileCreation(prj);
  }
}
inline static void ApplyTalentsToCast(Skill* skill) noexcept {
  for (const auto& te : UNIQUE_EFFECTS) {
    te->OnSpellCast(skill);
  }
}
inline static void ApplyTalentsToMonsterDeath(Monster* monster) noexcept {
  for (const auto& te : UNIQUE_EFFECTS) {
    te->OnMonsterDeath(monster);
  }
}
inline static void ApplyTalentsToPlayerHit(Projectile* prj) noexcept {
  for (const auto& te : UNIQUE_EFFECTS) {
    te->OnPlayerHit(prj);
  }
}



#endif  //MAGEQUEST_SRC_GAMEPLAY_UNIQUEEFFECT_H_
