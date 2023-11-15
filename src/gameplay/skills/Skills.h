#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_

struct BlastHammer_Skill final : public Skill {
  explicit BlastHammer_Skill(const SkillStats& stats)
      : Skill(stats, 1, textures::ui::skillbar::icons::blastHammer) {}

  void Activate(bool isFree) final { SkillAtMouse(skillStats.type, isFree); }
};
struct FireStrike_Skill final : public Skill {
  explicit FireStrike_Skill(const SkillStats& stats)
      : Skill(stats, 2, textures::ui::skillbar::icons::fireBurst) {}
  void Activate(bool isFree) final {
    SkillAtMouseRadial(skillStats.type, (int)skillStats.specialVal1, isFree);
  }
};
struct EnergySphere_Skill final : public Skill {
  explicit EnergySphere_Skill(const SkillStats& stats)
      : Skill(stats, 1, textures::ui::skillbar::icons::energy_sphere) {}
  void Activate(bool isFree) final { SkillToMouse(ENERGY_SPHERE, isFree); }
};
struct FireBall_Skill final : public Skill {
  explicit FireBall_Skill(const SkillStats& stats)
      : Skill(stats, 3, textures::ui::skillbar::icons::pyroBlast) {}
  void Activate(bool isFree) final { SkillToMouse(FIRE_BALL, false); }
};
struct Dummy_Skill final : public Skill {
  explicit Dummy_Skill() : Skill({}, 1, textures::EMPTY_TEXTURE) {}
  void Activate(bool isFree) final {}
};
struct LockedSlot_Skill final : public Skill {
  explicit LockedSlot_Skill() : Skill({}, 1, textures::ui::skillbar::icons::locked) {}
  void Activate(bool isFree) final {}
};
struct Lightning_Skill final : public Skill {
  explicit Lightning_Skill(const SkillStats& stats)
      : Skill(stats, 1, textures::ui::skillbar::icons::lightning) {}

  void Activate(bool isFree) final { SkillAtMouse(skillStats.type, isFree); }
};
struct FrostNova_Skill final : public Skill {
  explicit FrostNova_Skill(const SkillStats& stats)
      : Skill(stats, 1, textures::ui::skillbar::icons::frostNova) {}
  void Activate(bool isFree) final { SkillAtPlayer(FROST_NOVA, isFree); }
};
struct ArcaneBolt_Skill final : public Skill {
  explicit ArcaneBolt_Skill(const SkillStats& stats)
      : Skill(stats, 1, textures::ui::skillbar::icons::arcaneBolt) {}
  void Activate(bool isFree) final { SkillToMouse(ARCANE_BOLT, isFree); }
};
struct IceLance_Skill final : public Skill {
  explicit IceLance_Skill(const SkillStats& stats)
      : Skill(stats, 2, textures::ui::skillbar::icons::iceLance) {}
  void Activate(bool isFree) final { SkillToMouse(ICE_LANCE, isFree); }
};
struct GlacialBurst_Skill final : public Skill {
  explicit GlacialBurst_Skill(const SkillStats& stats)
      : Skill(stats, 2, textures::ui::skillbar::icons::glacialBurst) {}
  void Activate(bool isFree) final { SkillAtMouse(GLACIAL_BURST, isFree); }
};
struct InfernoRay_Skill final : public Skill {
  explicit InfernoRay_Skill(const SkillStats& stats)
      : Skill(stats, 1, textures::ui::skillbar::icons::infernoRay) {}
  void Activate(bool isFree) final { SkillToMouse(INFERNO_RAY, isFree); }
};
struct FireSword_Skill final : public Skill {
  explicit FireSword_Skill(const SkillStats& stats)
      : Skill(stats, 2, textures::ui::skillbar::icons::fireSword) {}
  void Activate(bool isFree) final { SkillAtPlayer(FIRE_SWORD, isFree); }
};
struct VoidField_Skill final : public Skill {
  explicit VoidField_Skill(const SkillStats& stats)
      : Skill(stats, 2, textures::ui::skillbar::icons::voidField) {}
  void Activate(bool isFree) final { SkillAtMouse(VOID_FIELD, isFree); }
};
struct VoidEruption_Skill final : public Skill {
  explicit VoidEruption_Skill(const SkillStats& stats)
      : Skill(stats, 2, textures::ui::skillbar::icons::voidEruption) {}
  void Activate(bool isFree) final { SkillAtMouse(VOID_ERUPTION, isFree); }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
