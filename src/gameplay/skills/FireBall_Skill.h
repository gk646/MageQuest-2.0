#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_

struct FireBall_Skill final : public Skill {
  FireBall_Skill(bool from_player, float damage)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::FIRE, damage}, from_player,
              1) {}

  void activate() final {
    use();

    if (MP_TYPE == MultiplayerType::CLIENT || MP_TYPE == MultiplayerType::SERVER) {
      auto mouse_pos = MOUSE_POS;
      float angle = std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y),
                               mouse_pos.x - (PLAYER_X + DRAW_X));
      Multiplayer::send_event(
          UDP_PROJECTILE,
          new UDP_Projectile(FIRE_BALL, PLAYER_X, PLAYER_Y, angle * (180.0f / M_PI),
                             std::cos(angle), std::sin(angle), damage_stats.damage));

      if (MP_TYPE == MultiplayerType::CLIENT) {
        return;
      }
    }
    PROJECTILES.push_back(
        new FireBall({PLAYER_X, PLAYER_Y}, true,
                     PLAYER_STATS.get_weapon_damage(damage_stats.damage_type)));
  }

  inline void update() final { cool_down_ticks++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
