#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
//Metaclass that handles display and activation of projectiles and effects
struct Skill {
  inline static constexpr float SKILL_ICON_SIZE = 50;
  inline static constexpr float TOOL_TIP_WIDTH = 220;
  inline static constexpr float TOOL_TIP_BASE_HEIGHT = 93;
  inline static constexpr int GLOBAL_COOLDOWN_TICKS = 30;
  //Only one cast can be active at a time
  inline static int16_t castProgress = -1;
  inline static Skill* lastCastedSkill = nullptr;
  //Global cooldown
  inline static int16_t globalCooldown = 0;
  std::string name;
  std::string description;
  SkillStats skillStats;
  DamageStats damageStats;
  const Texture& icon;
  uint16_t coolDownUpCounter;
  uint8_t attackAnimation = 0;
  bool activated = false;
  Skill(const SkillStats& skillStats, const DamageStats& damageStats,
        int attack_animation, const Texture& icon) noexcept
      : damageStats(damageStats),
        skillStats(skillStats),
        attackAnimation(attack_animation),
        coolDownUpCounter((int16_t)skillStats.coolDownTicks),
        icon(icon) {}

 public:
  //Draws the cast bar
  inline static void DrawCastBar() noexcept {
    if (!lastCastedSkill) return;
    float x = (SCREEN_WIDTH - 92) / 2;
    float y = SCREEN_HEIGHT * 0.75F;

    DrawRectangleProFast(
        x + 3, y + 3,
        ((float)castProgress / (float)lastCastedSkill->skillStats.castTime) * 79.0F, 4,
        Colors::castBarOrange);
    DrawTextureProFast(textures::ui::skillbar::castbar, x, y, 0, WHITE);
  }
  //Updates the cast bar and global cooldown // called on update tick
  inline static void UpdateStaticState() noexcept;

 public:
  //Activates the skill
  inline virtual void Activate(bool isFree) = 0;
  //Draws the icon and the cooldown effect if applicable
  inline void Draw(float x, float y) noexcept {
    DrawTextureProFast(icon, x, y, 0, WHITE);
    Util::DrawSwipeCooldownEffect(x, y, SKILL_ICON_SIZE,
                                  (int)PLAYER_STATS.GetTotalCD(skillStats),
                                  coolDownUpCounter);
    if (coolDownUpCounter < 60) return;
    Util::DrawSwipeCooldownEffect(x, y, SKILL_ICON_SIZE, GLOBAL_COOLDOWN_TICKS,
                                  globalCooldown);
  }
  //Updates the skill // only progresses cooldown
  inline void Update() noexcept { coolDownUpCounter++; }
  //TODO proper link to support bar
  static inline void DrawSupportBar(float x, float y, float percent) noexcept {
    DrawRectangleProFast(x - SCALE(2), y - SCALE(12), SCALE(53) * percent, SCALE(7),
                         Colors::SUPPORT_BAR_ORANGE);
  }

 public:
  //Returns true if skill is ready to use
  [[nodiscard]] inline bool IsUsable() const noexcept {
    return globalCooldown == GLOBAL_COOLDOWN_TICKS &&
           PLAYER_STATS.IsSkillUsable(skillStats, coolDownUpCounter);
  }
  //Returns a ptr to a new skill with the given stats / Projectile type is used to identify skills
  inline static Skill* GetNewSkill(ProjectileType type, const SkillStats& stats) noexcept;
  //Draws the tooltip for the skill
  inline void DrawTooltip(float x, float y) noexcept {
    DrawRectangleProFast(x + 1, y + 1, SKILL_ICON_SIZE, SKILL_ICON_SIZE,
                         Colors::lightGreyMiddleAlpha);
    DrawRangeCircle();
    int lineBreaks = 0;
    auto descriptionText = Util::WrapText(
        Util::CreateToolTipString(description, PLAYER_STATS.GetAbilityDmg(damageStats),
                                  skillStats.specialVal1, skillStats.specialVal2),
        TOOL_TIP_WIDTH, MINECRAFT_REGULAR, 15, &lineBreaks);

    float toolTipHeight = TOOL_TIP_BASE_HEIGHT + (float)lineBreaks * 15.0F;
    float startX = MOUSE_POS.x - TOOL_TIP_WIDTH / 2;
    float startY = MOUSE_POS.y - toolTipHeight - 3;
    DrawRectangleRounded({startX, startY, TOOL_TIP_WIDTH, toolTipHeight}, 0.1F,
                         ROUND_SEGMENTS, Colors::mediumLightGrey);
    DrawRectangleRoundedLines({startX, startY, TOOL_TIP_WIDTH, toolTipHeight}, 0.1F,
                              ROUND_SEGMENTS, 2, damageTypeToColor[damageStats.dmgType]);

    //-----------Description-----------//

    DrawTextExR(MINECRAFT_REGULAR, descriptionText.c_str(), {startX + 5, startY + 75}, 15,
                0.5F, Colors::descriptionOrange);

    //-----------Name-----------//
    DrawTextExR(MINECRAFT_BOLD, name.c_str(), {startX + 5, startY + 5}, 20, 0.5F,
                damageTypeToColor[damageStats.dmgType]);

    if (skillStats.type == DUMMY || skillStats.type == LOCKED) return;

    //-------------ResourceCost---------------//
    if (skillStats.manaCost > 0) {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%i Mana", (int)skillStats.manaCost);
      DrawTextExR(MINECRAFT_REGULAR, TEXT_BUFFER, {startX + 5, startY + 35}, 15, 0.5F,
                  Colors::darkBackground);
    }

    //-----------CastTime-----------//

    if (skillStats.castTime == 0) {
      DrawTextExR(MINECRAFT_REGULAR, "Instant", {startX + 5, startY + 53}, 15, 0.5F,
                  Colors::darkBackground);
    } else {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.1f sec cast",
               (float)skillStats.castTime / 60.0F);
      DrawTextExR(MINECRAFT_REGULAR, TEXT_BUFFER, {startX + 5, startY + 53}, 15, 0.5F,
                  Colors::darkBackground);
    }

    //-----------CoolDown-----------//

    if (skillStats.coolDownTicks > 0) {
      float cooldownInSeconds = (float)skillStats.coolDownTicks / 60.0F;

      if (cooldownInSeconds == floor(cooldownInSeconds)) {
        snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.0f sec cooldown", cooldownInSeconds);
      } else {
        snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%.1f sec cooldown", cooldownInSeconds);
      }

      Util::DrawRightAlignedText(MINECRAFT_REGULAR, 15, TEXT_BUFFER,
                                 startX + TOOL_TIP_WIDTH - 5, startY + 53,
                                 Colors::darkBackground);
    }
  }

 private:
  //Handles logic for when the skill is used
  inline void UseResources(bool isFree) noexcept;
  //Does a ray-cast and range-check from soundPlayer to mouse position
  [[nodiscard]] inline bool RangeLineOfSightCheck(const Point& target) const noexcept;
  //Draws the range indicator circle
  inline void DrawRangeCircle() const noexcept {
    if (skillStats.range > 0) {
      DrawTextureScaled(
          textures::ui::skillbar::skillRange,
          {(CAMERA_X + 14) - (float)skillStats.range, (CAMERA_Y + 25) - skillStats.range,
           skillStats.range * 2.05F, skillStats.range * 2.05F},
          0, false, 0, WHITE);
    }
  }
  //Handles the casting logic and initiates the cast
  [[nodiscard]] inline bool HandleCasting(bool isFree) noexcept;
  //Returns the scaled damage based on type and player stats
  [[nodiscard]] inline float GetSkillDamage(ProjectileType type) const noexcept {
    return PLAYER_STATS.GetAbilityDmg(damageStats) /
           (typeToInfo[type].hitType == HitType::CONTINUOUS ? 60.0F : 1.0F);
  }
  inline static void ResetCast() noexcept {
    lastCastedSkill = nullptr;
    castProgress = -1;
    StopSound(sound::player::abilityCast);
  }

 protected:
  inline void SkillAtMouse(ProjectileType type, bool isFree) noexcept;
  inline void SkillAtMouseRadial(ProjectileType type, int numProjectiles,
                                 bool isFree) noexcept;
  inline void SkillToMouse(ProjectileType type, bool isFree) noexcept;
  inline void SkillAtPlayer(ProjectileType type, bool isFree) noexcept;
};

#include "../gameobjects/projectiles/Projectiles.h"

inline static Projectile* GetProjectileInstance(
    ProjectileType type, const Point& pos, bool isFriendlyToPlayer, float damage,
    const Entity* sender, const Vector2& mvmt, int16_t pov,
    const std::array<StatusEffect*, MAX_STATUS_EFFECTS_PRJ>& effects = {},
    const Sound& sound = sound::EMPTY_SOUND) noexcept {
  switch (type) {
    case POISON_BALL:
      break;
    case FIRE_STRIKE:
      return new FireBall(pos, isFriendlyToPlayer, damage, effects, pov, mvmt, sender,
                          &sound == &sound::EMPTY_SOUND);
    case FIRE_STRIKE_II:
      break;
    case FIRE_BALL:
      return new FireBall(pos, isFriendlyToPlayer, damage, effects, pov, mvmt, sender,
                          &sound == &sound::EMPTY_SOUND);
    case ARROW_NORMAL:
      return new ArrowNormal(pos, isFriendlyToPlayer, damage, effects, mvmt, sound,
                             sender);
    case BLAST_HAMMER:
      return new BlastHammer(pos, isFriendlyToPlayer, damage, effects, sender);
    case ENERGY_SPHERE:
      return new EnergySphere(pos, isFriendlyToPlayer, damage, effects, mvmt, sender);
    case FIRE_SWORD:
      return new FireSword(pos, isFriendlyToPlayer, damage, effects, pov, mvmt, sender);
    case FROST_NOVA:
      return new FrostNova(pos, isFriendlyToPlayer, damage, effects, {0, 0}, sender);
    case ICE_LANCE:
      return new IceLance(pos, isFriendlyToPlayer, damage, effects, pov, mvmt, sender);
    case INFERNO_RAY:
      return new InfernoRay(pos, isFriendlyToPlayer, damage, effects, pov, mvmt, sender);
    case LIGHTNING:
      return new Lightning(pos, isFriendlyToPlayer, damage, effects, mvmt, sender);
    case PYRO_BLAST:
      break;
    case SOLAR_FLARE:
      break;
    case THUNDER_SPLASH:
      break;
    case THUNDER_STRIKE:
      break;
    case VOID_ERUPTION:
      return new VoidEruption(pos, isFriendlyToPlayer, damage, effects, {0, 0}, sender);
    case VOID_FIELD:
      return new VoidField(pos, isFriendlyToPlayer, damage, effects, {0, 0}, sender);
    case ARCANE_BOLT:
      return new ArcaneBolt(pos, isFriendlyToPlayer, damage, effects, pov, mvmt, sender);
    case PROJECTILE_END:
      break;
    case PSYCHIC_SCREAM:
      return new PsychicScream(pos, isFriendlyToPlayer, damage);
    case GLACIAL_BURST:
      return new GlacialBurst(pos, isFriendlyToPlayer, damage, effects, {0, 0}, sender);
    case DUMMY:
    case LOCKED:
      return nullptr;
  }
  std::cout << "MISSING PROJECTILE ENUM VAL:" << (int)type << std::endl;
  return nullptr;
}

#include "../gameobjects/components/AttackComponent.h"
#include "skills/Skills.h"
Skill* Skill::GetNewSkill(ProjectileType type, const SkillStats& stats) noexcept {
  switch (type) {
    case POISON_BALL:
      break;
    case FIRE_STRIKE:
    case FIRE_STRIKE_II:
      return new FireStrike_Skill(stats);
    case FIRE_BALL:
      return new FireBall_Skill(stats);
    case BLAST_HAMMER:
      return new BlastHammer_Skill(stats);
    case ENERGY_SPHERE:
      return new EnergySphere_Skill(stats);
    case FIRE_SWORD:
      return new FireSword_Skill(stats);
    case FROST_NOVA:
      return new FrostNova_Skill(stats);
    case ICE_LANCE:
      return new IceLance_Skill(stats);
    case INFERNO_RAY:
      return new InfernoRay_Skill(stats);
    case LIGHTNING:
      return new Lightning_Skill(stats);
    case PYRO_BLAST:
      break;
    case SOLAR_FLARE:
      break;
    case THUNDER_SPLASH:
      break;
    case THUNDER_STRIKE:
      break;
    case VOID_ERUPTION:
      return new VoidEruption_Skill(stats);
    case VOID_FIELD:
      return new VoidField_Skill(stats);
    case PROJECTILE_END:
      return nullptr;
    case ARROW_NORMAL:
      break;
    case ARCANE_BOLT:
      return new ArcaneBolt_Skill(stats);
    case PSYCHIC_SCREAM:
      break;
    case DUMMY:
      return new Dummy_Skill();
    case LOCKED:
      return new LockedSlot_Skill();
    case GLACIAL_BURST:
      return new GlacialBurst_Skill(stats);
  }
  std::cout << "MISSING SKILL ENUM VAL:" << (int)type << std::endl;
  return nullptr;
}
inline static void Multiplayer::HandleProjectile(UDP_Projectile* data,
                                                 const Entity* ptr) noexcept {
  switch (data->p_type) {
    case FIRE_BALL: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, data->damage,
          {nullptr, nullptr, nullptr}, data->pov, {data->move_x, data->move_y}, ptr));
      break;
    }
    case FIRE_STRIKE: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, data->damage,
          {nullptr, nullptr, nullptr}, data->pov, {data->move_x, data->move_y}, ptr));
      break;
    }
    case BLAST_HAMMER: {
      PROJECTILES.emplace_back(new BlastHammer({(float)data->x, (float)data->y},
                                               !FRIENDLY_FIRE, data->damage,
                                               {nullptr, nullptr, nullptr}, ptr));
      break;
    }
    case ENERGY_SPHERE: {
      PROJECTILES.emplace_back(new EnergySphere(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, data->damage,
          {nullptr, nullptr, nullptr}, {data->move_x, data->move_y}, ptr));
      break;
    }
  }
}

#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILL_H_
