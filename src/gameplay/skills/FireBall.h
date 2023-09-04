#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_

struct FireBall final : public Skill {
  FireBall(bool from_player, float damage)
      : Skill(SkillStats{300, 10, 0}, DamageStats{SourceType::MAGICAL, MagicType::FIRE, damage},
              from_player) {}

  void activate() final {
    use();
    PROJECTILES.emplace_back(FireBall_P(PLAYER.pos, true));
  }
  inline void update() final { cool_down_ticks++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
