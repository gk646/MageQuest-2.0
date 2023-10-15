#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_

struct FireStrike_Skill final : public Skill {
  static constexpr float BASE_DMG = 1;
  static constexpr int LIFE_SPAN = 200;
  static constexpr float SPEED = 2;
  int num_fireballs;
  FireStrike_Skill(bool from_player, int num_fireballs)
      : Skill(SkillStats{400, 10, 0, 420}, DamageStats{DamageType::FIRE, BASE_DMG},
              from_player, 2, textures::ui::skillbar::icons::fireBurst),
        num_fireballs(num_fireballs) {}

  void activate() final {
    if (!RangeLineOfSightCheck()) return;
    TriggerSkill();
    Point pos = {PLAYER_X + MOUSE_POS.x - CAMERA_X - FireBall::width / 2,
                 PLAYER_Y + MOUSE_POS.y - CAMERA_Y - FireBall::height / 2};
    const float interval_angle = 360.0f / num_fireballs;
    float damagePerFireball = PLAYER_STATS.get_ability_dmg(damage_stats) / 30.0F;
    for (int_fast32_t i = 0; i < num_fireballs; i++) {
      float angle_rad = interval_angle * i * DEG2RAD;
      float x_component = std::cos(angle_rad);
      float y_component = std::sin(angle_rad);
      float pov = angle_rad * RAD2DEG;

      Multiplayer::UDP_SEND_PROJECTILE(FIRE_STRIKE, pos.x_, pos.y_, pov, x_component,
                                       y_component, damagePerFireball);

      PROJECTILES.emplace_back(new FireBall(pos, from_player, LIFE_SPAN, SPEED,
                                            damagePerFireball, HitType::CONTINUOUS, {},
                                            pov, {x_component, y_component}, &PLAYER));
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
