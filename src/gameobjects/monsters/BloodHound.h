#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_BLOODHOUND_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_BLOODHOUND_H_
struct BloodHound final : public Monster {
  static constexpr int base_health = 3;
  static constexpr float per_level = 2.5;
  static constexpr float base_speed = 2.4;
  static constexpr float ATTACK_CD = 120;
  static constexpr uint8_t ATTACK_RANGE = 2;
  static constexpr uint8_t CHASE_RANGE = 10;
  BloodHound(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, base_speed},
                &textures::monsters::BLOOD_HOUND, MonsterType::WOLF, {45, 30}) {
    attack_cd = ATTACK_CD;
    AttackRange = ATTACK_RANGE;
    ChaseRange = CHASE_RANGE;
  }
  void draw() final {
    if (attack == -100) [[unlikely]] {
      draw_death();
    } else if (attack == 1) {
      draw_attack1();
    } else {
      if (moving) {
        DrawTextureProFastEx(resource->walk[sprite_counter % 60 / 10],
                             pos.x_ + DRAW_X-2, pos.y_ + DRAW_Y-19, 2, 0, !flip, WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[sprite_counter % 80 / 20], pos.x_ + DRAW_X-5,
                             pos.y_ + DRAW_Y-17, 5, 0, !flip, WHITE);
      }
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target)) {
      if ( attack == 0 && attack_cd < 0) {
        attack_cd = ATTACK_CD;
        sprite_counter = 0;
        attack = 1;
      } else {
        attack_cd--;
      }
    }
  }
  inline void draw_death() noexcept {
    int num = sprite_counter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X -5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = sprite_counter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 12,
                           pos.y_ + DRAW_Y - 19, 10, 0, !flip, WHITE);
    } else {
      attack = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_BLOODHOUND_H_
