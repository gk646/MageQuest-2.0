#ifndef MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
#define MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_

struct FireBall final : public Skill {
  FireBall(bool from_player, float damage)
      : Skill(SkillStats{300, 10, 0}, DamageStats{ DamageType::FIRE, damage},
              from_player) {}

  void activate() final {
    use();
    PROJECTILES.push_back(new FireBall_P({PLAYER_X,PLAYER_Y}, true));
  }
  inline void update() final { cool_down_ticks++; }
};
#endif  //MAGEQUEST_SRC_GAMEPLAY_ABILITIES_FIREBALL_H_
