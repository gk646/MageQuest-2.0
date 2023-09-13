#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_

struct FireBall final : public Skill {
  FireBall(bool from_player, float damage)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::FIRE, damage},
              from_player) {}

  void activate() final {
    use();
    if (MP_TYPE == MultiplayerType::CLIENT) {
      auto mouse_pos = GetMousePosition();
      float angle = std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y),
                               mouse_pos.x - (PLAYER_X + DRAW_X));
      Multiplayer::send_event(
          UDP_PROJECTILE_STATE,
          new UDP_Projectile(0, FIRE_BALL, PLAYER_X, PLAYER_Y, angle * (180.0f / M_PI),
                             std::cos(angle), std::sin(angle),damage_stats.damage));
      return;
    }
    PROJECTILES.push_back(new FireBall_P({PLAYER_X,PLAYER_Y}, true));
  }
  inline void update() final { cool_down_ticks++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
