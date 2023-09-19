#ifndef MAGEQUEST_SRC_GAMEPLAY_SKILLS_BLASTHAMMER_SKILL_H_
#define MAGEQUEST_SRC_GAMEPLAY_SKILLS_BLASTHAMMER_SKILL_H_
struct BlastHammer_Skill final : public Skill {
  static constexpr float base_dmg = 20;
  static constexpr float lifespan = 110;
  static constexpr float cooldown = 15*60;
  static constexpr float mana_cost = 20;
  explicit BlastHammer_Skill(bool from_player)
      : Skill(SkillStats{cooldown, mana_cost, 0}, DamageStats{DamageType::FIRE, base_dmg},
              from_player, 1) {}

  void activate() final {
    use();
    Point pos = {PLAYER_X + MOUSE_POS.x - CAMERA_X- BlastHammer::width/2, PLAYER_Y + MOUSE_POS.y - CAMERA_Y- BlastHammer::height/2};
    float damage = PLAYER_STATS.get_ability_dmg(damage_stats);

    Multiplayer::UDP_SEND_PROJECTILE(BLAST_HAMMER, pos.x_, pos.y_, 0, 0, 0, damage);
    if (MP_TYPE == MultiplayerType::CLIENT) return;

    PROJECTILES.emplace_back(new BlastHammer(pos, from_player, lifespan, 0,
                                          damage, HitType::ONE_TICK, {}, 0,
                                          {0, 0},pos.x_ < MIRROR_POINT));
  }

};
#endif  //MAGEQUEST_SRC_GAMEPLAY_SKILLS_BLASTHAMMER_SKILL_H_
