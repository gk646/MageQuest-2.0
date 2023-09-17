#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_

struct FireStrike_Skill final : public Skill {
  static constexpr float base_dmg = 1 / 30.0F;
  static constexpr int lifespan = 120;
  static constexpr float speed = 2;
  int num_fireballs;
  FireStrike_Skill(bool from_player, int num_fireballs)
      : Skill(SkillStats{400, 10, 0}, DamageStats{DamageType::FIRE, base_dmg},
              from_player, 2),
        num_fireballs(num_fireballs) {}

  void activate() final {
    use();
    Point pos = {PLAYER_X + MOUSE_POS.x - CAMERA_X, PLAYER_Y + MOUSE_POS.y - CAMERA_Y};
    const float interval_angle = 360.0f / num_fireballs;
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats);
    for (uint_fast32_t i = 0; i < num_fireballs; i++) {
      float angle_rad = (interval_angle * i) * (M_PI / 180.0f);

      float x_component = std::cos(angle_rad);
      float y_component = std::sin(angle_rad);

      float pov = angle_rad * (180.0f / M_PI);

      SEND_UDP_PROJECTILE(FIRE_STRIKE, pos.x_, pos.y_, pov, x_component, y_component,
                          damage_stats.damage);
      if (MP_TYPE == MultiplayerType::CLIENT) continue;
      PROJECTILES.emplace_back(new FireBall(pos, from_player, lifespan, speed, damage,
                                            HitType::CONTINUOUS, {}, pov,
                                            {x_component, y_component}));
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
