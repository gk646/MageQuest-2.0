#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_

struct Dummy_Skill final : public Skill {
  static constexpr float base_dmg = 0.5;
  explicit Dummy_Skill(bool from_player = true)
      : Skill(SkillStats{300, 10, 0,0}, DamageStats{DamageType::FIRE, base_dmg},
              from_player, 1, textures::EMPTY_TEXTURE) {}

  void Activate() final {
    TriggerSkill();

    PROJECTILES.push_back(new Dummy({PLAYER_X + PLAYER.size.x_ / 2,
                                     PLAYER_Y + (PLAYER.size.y_ - FireBall::height) / 2},
                                    true, PLAYER_STATS.get_ability_dmg(damage_stats)));
  }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_DUMMY_H_
