#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_

struct FireStrike_Skill final : public Skill {
  int num_fireballs;
  FireStrike_Skill(bool from_player, float damage, int num_fireballs)
      : Skill(SkillStats{400, 20, 0}, DamageStats{DamageType::FIRE, damage}, from_player,
              2),
        num_fireballs(num_fireballs) {}

  void activate() final {
    use();
    auto mouse_pos = MOUSE_POS;
    Point pos = {PLAYER_X + mouse_pos.x - CAMERA_X, PLAYER_Y + mouse_pos.y - CAMERA_Y};
    const float interval_angle = 360.0f / num_fireballs;
    float damage = PLAYER_STATS.get_weapon_damage(DamageType::FIRE);
    for (uint_fast32_t i = 0; i < num_fireballs; i++) {
      float angle_rad = (interval_angle * i) * (M_PI / 180.0f);

      float x_component = std::cos(angle_rad);
      float y_component = std::sin(angle_rad);

      float pov = angle_rad * (180.0f / M_PI);
      Multiplayer::send_event(
          UDP_PROJECTILE,
          new UDP_Projectile(FIRE_STRIKE, pos.x_, pos.y_, pov, x_component, y_component,
                             damage_stats.damage));
      if (MP_TYPE == MultiplayerType::CLIENT) {
        continue;
      }
      PROJECTILES.emplace_back(new FireBall(pos, from_player, 120, 2, damage,
                                            HitType::CONTINUOUS, {}, pov,
                                            {x_component, y_component}));
    }
  }
  inline void update() final { cool_down_ticks++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
