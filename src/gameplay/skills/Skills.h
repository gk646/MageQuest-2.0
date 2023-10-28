#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
struct BlastHammer_Skill final : public Skill {
  explicit BlastHammer_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::FIRE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::blastHammer) {}

  void Activate() final {
    if (!RangeLineOfSightCheck()) return;
    TriggerSkill();
    Point pos = {PLAYER_X + MOUSE_POS.x - CAMERA_X - BlastHammer::width / 2.0F,
                 PLAYER_Y + MOUSE_POS.y - CAMERA_Y - BlastHammer::height / 2.0F};
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats);

    Multiplayer::UDP_SEND_PROJECTILE(BLAST_HAMMER, (int16_t)pos.x_, (int16_t)pos.y_, 0, 0,
                                     0, damage);

    PROJECTILES.emplace_back(new BlastHammer(pos, true, skillStats.lifeSpan, 0, damage,
                                             HitType::ONE_TICK, {}, 0, {0, 0},
                                             pos.x_ < MIRROR_POINT, &PLAYER));
  }
};
struct FireStrike_Skill final : public Skill {
  explicit FireStrike_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::FIRE, stats.baseDamage}, from_player, 2,
              textures::ui::skillbar::icons::fireBurst) {}
  void Activate() final {
    if (!RangeLineOfSightCheck()) return;
    TriggerSkill();
    Point pos = {PLAYER_X + MOUSE_POS.x - CAMERA_X - FireBall::width / 2,
                 PLAYER_Y + MOUSE_POS.y - CAMERA_Y - FireBall::height / 2};
    const float interval_angle = 360.0f / skillStats.specialVal1;
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats) / 60.0F;
    for (int_fast32_t i = 0; i < skillStats.specialVal1; i++) {
      float angle_rad = interval_angle * i * DEG2RAD;
      float x_component = std::cos(angle_rad);
      float y_component = std::sin(angle_rad);
      float pov = angle_rad * RAD2DEG;

      Multiplayer::UDP_SEND_PROJECTILE(FIRE_STRIKE, (int16_t)pos.x_, (int16_t)pos.y_, pov,
                                       x_component, y_component, damage);

      PROJECTILES.emplace_back(new FireBall(
          pos, true, skillStats.lifeSpan, skillStats.speed, damage, HitType::CONTINUOUS,
          {}, pov, {x_component, y_component}, &PLAYER, i > 0));
    }
  }
};
struct EnergySphere_Skill final : public Skill {
  explicit EnergySphere_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ARCANE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::energy_sphere) {}
  void Draw(float x, float y, float size) noexcept final {
    if (coolDownUpCounter < skillStats.lifeSpan) {
      DrawSupportBar(x, y, 1.0F - (float)coolDownUpCounter / skillStats.lifeSpan);
    }
    Skill::Draw(x, y, size);
  }
  void Activate() final {
    TriggerSkill();
    float posX = PLAYER_X + PLAYER.size.x / 2;
    float posY = PLAYER_Y + (PLAYER.size.y - EnergySphere::HEIGHT) / 2;
    float angle = std::atan2(MOUSE_POS.y - posY - DRAW_Y - EnergySphere::HEIGHT / 2,
                             MOUSE_POS.x - posX - DRAW_X - EnergySphere::WIDTH / 2);
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats) / 60.0F;
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(ENERGY_SPHERE, static_cast<int16_t>(posX),
                                     static_cast<int16_t>(posY), 0, x_move, y_move,
                                     damage);

    PROJECTILES.emplace_back(new EnergySphere({posX, posY}, from_player,
                                              skillStats.lifeSpan, skillStats.speed,
                                              damage, {}, {x_move, y_move}, &PLAYER));
  }
};
struct FireBall_Skill final : public Skill {
  explicit FireBall_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::FIRE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::pyroBlast) {}
  void Activate() final {
    TriggerSkill();
    auto mouse_pos = MOUSE_POS;
    float angle =
        std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y), mouse_pos.x - (PLAYER_X + DRAW_X));
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats);
    float posX = PLAYER_X + PLAYER.size.x / 2;
    float posY = PLAYER_Y + PLAYER.size.y / 2 - FireBall::height / 2;
    float pov = angle * (180.0f / PI);
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(FIRE_BALL, (int16_t)posX, (int16_t)posY, pov, x_move,
                                     y_move, damage);

    PROJECTILES.emplace_back(new FireBall({posX, posY}, true, skillStats.lifeSpan,
                                          skillStats.speed, damage, HitType::ONE_HIT, {},
                                          pov, {x_move, y_move}, &PLAYER));
  }
};
struct Dummy_Skill final : public Skill {
  static constexpr float base_dmg = 0.5;
  explicit Dummy_Skill(const SkillStats& stats = {})
      : Skill(stats, DamageStats{DamageType::FIRE, base_dmg}, from_player, 1,
              textures::EMPTY_TEXTURE) {}
  void Activate() final {
    //no skill triggered
  }
};
struct LockedSlot_Skill final : public Skill {
  explicit LockedSlot_Skill(const SkillStats& stats = {})
      : Skill({}, DamageStats{DamageType::TRUE_DMG, 0}, from_player, 1,
              textures::ui::skillbar::icons::locked) {}
  void Activate() final {}
};
struct Lightning_Skill final : public Skill {
  explicit Lightning_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ARCANE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::lightning) {}

  void Activate() final {
    if (!RangeLineOfSightCheck()) return;
    TriggerSkill();
    Point pos = {PLAYER_X + MOUSE_POS.x - CAMERA_X - Lightning::WIDTH / 2,
                 PLAYER_Y + MOUSE_POS.y - CAMERA_Y - Lightning::HEIGHT / 2};
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats);

    Multiplayer::UDP_SEND_PROJECTILE(LIGHTNING, (int16_t)pos.x_, (int16_t)pos.y_, 0, 0, 0,
                                     damage);

    PROJECTILES.emplace_back(new Lightning(pos, true, skillStats.lifeSpan, 0, damage,
                                           HitType::ONE_TICK, {}, 0, {0, 0},
                                           pos.x_ < MIRROR_POINT, &PLAYER));
  }
};
struct FrostNova_Skill final : public Skill {
  explicit FrostNova_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ICE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::frostNova) {}
  void Activate() final {
    TriggerSkill();
    Point pos = {PLAYER_X + PLAYER.size.x / 2 - FrostNova::WIDTH / 2,
                 PLAYER_Y + PLAYER.size.y / 2 - FrostNova::HEIGHT / 2};
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats);
    Multiplayer::UDP_SEND_PROJECTILE(FROST_NOVA, (int16_t)pos.x_, (int16_t)pos.y_, 0, 0,
                                     0, damage);

    PROJECTILES.emplace_back(new FrostNova(pos, true, skillStats.lifeSpan, 0, damage,
                                           HitType::ONE_TICK, {}, 0, {0, 0}, &PLAYER));
  }
};

struct ArcaneBolt_Skill final : public Skill {
  explicit ArcaneBolt_Skill(const SkillStats& stats)
      : Skill(stats, DamageStats{DamageType::ARCANE, stats.baseDamage}, true, 1,
              textures::ui::skillbar::icons::arcaneBolt) {}
  void Activate() final {
    TriggerSkill();
    auto mouse_pos = MOUSE_POS;
    float angle =
        std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y), mouse_pos.x - (PLAYER_X + DRAW_X));
    float damage = PLAYER_STATS.GetAbilityDmg(damageStats);
    float posX = PLAYER_X + PLAYER.size.x / 2;
    float posY = PLAYER_Y + PLAYER.size.y / 2 - ArcaneBolt::height / 2;
    float pov = angle * (180.0f / PI);
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(ARCANE_BOLT, (int16_t)posX, (int16_t)posY, pov,
                                     x_move, y_move, damage);

    PROJECTILES.emplace_back(new ArcaneBolt({posX, posY}, true, skillStats.lifeSpan,
                                            skillStats.speed, damage, HitType::ONE_HIT,
                                            {}, pov, {x_move, y_move}, &PLAYER));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_SKILLS_H_
