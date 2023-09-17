#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_

struct Dummy_Skill final : public Skill {
  static constexpr float base_dmg = 0.5;
  explicit Dummy_Skill(bool from_player = true)
      : Skill(SkillStats{300, 10, 0}, DamageStats{DamageType::FIRE, base_dmg},
              from_player, 1) {}

  void activate() final {
    use();

    PROJECTILES.push_back(
        new Dummy({PLAYER.pos.x_ + PLAYER.size.x_ / 2,
                   PLAYER.pos.y_ + PLAYER.size.y_ / 2 - FireBall::height / 2},
                  true, PLAYER_STATS.get_ability_dmg(damage_stats)));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_
