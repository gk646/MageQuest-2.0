#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
struct EnergySphere_Skill final : public Skill {
  static constexpr float base_dmg = 1;
  static constexpr float lifespan = 180;
  static constexpr float speed = 2.5;
  explicit EnergySphere_Skill(bool from_player)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::FIRE, base_dmg},
              from_player, 1) {}

  void activate() final {
    use();
    auto mouse_pos = MOUSE_POS;
    float angle =
        std::atan2(mouse_pos.y - (PLAYER_Y + DRAW_Y), mouse_pos.x - (PLAYER_X + DRAW_X));
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats) / 50.0F;
    float posX = PLAYER_X + PLAYER.size.x_ / 2;
    float posY = PLAYER_Y + PLAYER.size.y_ / 2 - EnergySphere::height / 2;
    float pov = angle * (180.0f / M_PI);
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(ENERGY_SPHERE, posX, posY, pov, x_move, y_move,
                                     damage);
    if (MP_TYPE == MultiplayerType::CLIENT) return;

    PROJECTILES.emplace_back(new EnergySphere({posX, posY}, from_player, lifespan, speed,
                                              damage, HitType::CONTINUOUS, {}, pov,
                                              {x_move, y_move}));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
