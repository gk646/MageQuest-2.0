#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
struct EnergySphere_Skill final : public Skill {
  static constexpr float BASE_DMG = 1;
  static constexpr float LIFESPAN = 280;
  static constexpr float SPEED = 1.5F;
  static constexpr float CONTINUOUS_DIVISOR = 50;
  explicit EnergySphere_Skill(bool from_player)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::ARCANE, BASE_DMG},
              from_player, 1, textures::ui::skillbar::icons::energy_sphere) {}
  void Draw(float x, float y, float size)  noexcept final {
    if(cool_down_ticks < LIFESPAN){
      DrawSupportBar(x, y, 1- cool_down_ticks / LIFESPAN);
    }
    Skill::Draw(x, y, size);
  }
  void Activate() final {
    TriggerSkill();
    float posX = PLAYER_X + PLAYER.size.x_ / 2;
    float posY = PLAYER_Y + (PLAYER.size.y_ - EnergySphere::HEIGHT) / 2;
    float angle = std::atan2(MOUSE_POS.y - posY - DRAW_Y  - EnergySphere::HEIGHT/2,
                             MOUSE_POS.x - posX  -DRAW_X - EnergySphere::WIDTH/2);
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats) / CONTINUOUS_DIVISOR;
    float x_move = std::cos(angle);
    float y_move = std::sin(angle);

    Multiplayer::UDP_SEND_PROJECTILE(ENERGY_SPHERE, static_cast<int16_t>(posX),
                                     static_cast<int16_t>(posY), 0, x_move, y_move,
                                     damage);

    PROJECTILES.emplace_back(new EnergySphere({posX, posY}, from_player, LIFESPAN, SPEED,
                                              damage, HitType::CONTINUOUS, {},
                                              {x_move, y_move},&PLAYER));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_ENERGYSPHERE_SKILL_H_
