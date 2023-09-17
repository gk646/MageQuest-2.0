#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_

struct FireBall_Skill final : public Skill {
  static constexpr float base_dmg = 2;
  static constexpr float lifespan = 250;
  static constexpr float speed = 3;
  explicit FireBall_Skill(bool from_player)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::FIRE, base_dmg},
              from_player, 1) {}

  void activate() final {
    use();
    auto mouse_pos = MOUSE_POS;
    float angle =
        std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y), mouse_pos.x - (PLAYER_X + DRAW_X));
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats);
    float posX = PLAYER_X + PLAYER.size.x_;
    float posY = PLAYER_Y + PLAYER.size.y_ - FireBall::height / 2;
    float pov = angle * (180.0f / M_PI);
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    SEND_UDP_PROJECTILE(FIRE_BALL, posX, posY, pov, x_move, y_move, damage);
    if (MP_TYPE == MultiplayerType::CLIENT) return;

    PROJECTILES.emplace_back(new FireBall({posX, posY}, from_player, lifespan, speed,
                                          damage, HitType::ONE_HIT, {}, pov,
                                          {x_move, y_move}));
  }

};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
