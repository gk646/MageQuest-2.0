#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_

struct BlastHammer_Skill final : public Skill {
  explicit BlastHammer_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::FIRE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::blastHammer) {}

  void Activate(bool isFree) final { SkillAtMouse(skillStats.type, isFree); }
};
struct FireStrike_Skill final : public Skill {
  explicit FireStrike_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::FIRE, stats.baseDamage}, true, 2,
              textures::ui::skillbar::icons::fireBurst) {}
  void Activate(bool isFree) final {
    SkillAtMouseRadial(skillStats.type, (int)skillStats.specialVal1, isFree);
  }
};
struct EnergySphere_Skill final : public Skill {
  explicit EnergySphere_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ARCANE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::energy_sphere) {}
  void Draw(float x, float y, float size) noexcept final {
    if (coolDownUpCounter < 200) {
      DrawSupportBar(x, y, 1.0F - (float)coolDownUpCounter / 200);
    }
    Skill::Draw(x, y, size);
  }
  void Activate(bool isFree) final { SkillToMouse(ENERGY_SPHERE, isFree); }
};
struct FireBall_Skill final : public Skill {
  explicit FireBall_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::FIRE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::pyroBlast) {}
  void Activate(bool isFree) final {}
};
struct Dummy_Skill final : public Skill {
  static constexpr float base_dmg = 0.5;
  explicit Dummy_Skill(const SkillStats& stats = {})
      : Skill(stats, DamageStats{DamageType::FIRE, base_dmg}, true, 1,
              textures::EMPTY_TEXTURE) {}
  void Activate(bool isFree) final {
    //no skill triggered
  }
};
struct LockedSlot_Skill final : public Skill {
  explicit LockedSlot_Skill()
      : Skill({}, DamageStats{DamageType::TRUE_DMG, 0}, true, 1,
              textures::ui::skillbar::icons::locked) {}
  void Activate(bool isFree) final {}
};
struct Lightning_Skill final : public Skill {
  explicit Lightning_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ARCANE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::lightning) {}

  void Activate(bool isFree) final { SkillAtMouse(skillStats.type, isFree); }
};
struct FrostNova_Skill final : public Skill {
  explicit FrostNova_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ICE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::frostNova) {}
  void Activate(bool isFree) final { SkillAtPlayer(FROST_NOVA, isFree); }
};
struct ArcaneBolt_Skill final : public Skill {
  explicit ArcaneBolt_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ARCANE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::arcaneBolt) {}
  void Activate(bool isFree) final { SkillToMouse(ARCANE_BOLT, isFree); }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
