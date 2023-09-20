#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
struct EnergySphere_Skill final : public Skill {
  static constexpr float BASE_DMG = 1;
  static constexpr float LIFESPAN = 180;
  static constexpr float SPEED = 2;
  static constexpr float CONTINUOUS_DIVISOR = 50;
  explicit EnergySphere_Skill(bool from_player)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::ARCANE, BASE_DMG},
              from_player, 1) {}

  void activate() final {
    use();
    auto mouse_pos = MOUSE_POS;
    float angle = std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y) - EnergySphere::width,
                             mouse_pos.x - (PLAYER_X + DRAW_X) - EnergySphere::height);
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats) / CONTINUOUS_DIVISOR;
    float posX = PLAYER_X + PLAYER.size.x_ / 2;
    float posY = PLAYER_Y + PLAYER.size.y_ / 2 - EnergySphere::height / 2;
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(ENERGY_SPHERE, static_cast<int16_t>(posX),
                                     static_cast<int16_t>(posY), 0, x_move, y_move,
                                     damage);

    PROJECTILES.emplace_back(new EnergySphere({posX, posY}, from_player, LIFESPAN, SPEED,
                                              damage, HitType::CONTINUOUS, {},
                                              {x_move, y_move}));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
