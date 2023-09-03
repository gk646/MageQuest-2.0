#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_

struct FireStrike final : public Skill {
  bool from_player;
  int num_fireballs;
  FireStrike(bool from_player, float damage, int num_fireballs)
      : Skill(SkillStats{400, 20, 0}, DamageStats{SourceType::MAGICAL, MagicType::FIRE, damage}),
        from_player(from_player),
        num_fireballs(num_fireballs) {}

  void activate() final {
    use();
    auto mouse_pos = GetMousePosition();
    Point pos = {PLAYER_X + mouse_pos.x - CAMERA_X, PLAYER_Y + mouse_pos.y - CAMERA_Y};
    const float interval_angle = 360.0f / num_fireballs;
    for (uint_fast32_t i = 0; i < num_fireballs; i++) {
      // Convert the current angle to radians
      float angle_rad = (interval_angle * i) * (M_PI / 180.0f);

      // Calculate the x and y components based on the angle
      float x_component = std::cos(angle_rad);
      float y_component = std::sin(angle_rad);

      // Create the fireball with this movement vector
      PROJECTILES.push_back(Projectile(from_player, pos, {25, 25}, ShapeType::RECT, 300, 4,
                                       damage_stats, ProjectileType::CONTINUOUS,
                                       {new Burn{1, 1, 1}}, {x_component, y_component}));
    }
  }
  inline void update() final {
    std::cout << cool_down_ticks << std::endl;
    cool_down_ticks++;
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIRESTRIKE_H_
