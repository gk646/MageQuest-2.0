#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_

struct FireBall_Skill final : public Skill {
  static constexpr float BASE_DMG = 2;
  static constexpr float LIFE_SPAN = 250;
  static constexpr float SPEED = 3;
  explicit FireBall_Skill(bool from_player)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::FIRE, BASE_DMG},
              from_player, 1) {}

  void activate() final {
    use();
    auto mouse_pos = MOUSE_POS;
    float angle =
        std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y), mouse_pos.x - (PLAYER_X + DRAW_X));
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats);
    float posX = PLAYER_X + PLAYER.size.x_ / 2;
    float posY = PLAYER_Y + PLAYER.size.y_ / 2 - FireBall::height / 2;
    float pov = angle * (180.0f / M_PI);
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(FIRE_BALL, posX, posY, pov, x_move, y_move, damage);

    PROJECTILES.emplace_back(new FireBall({posX, posY}, from_player, LIFE_SPAN, SPEED,
                                          damage, HitType::ONE_HIT, {}, pov,
                                          {x_move, y_move}));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
