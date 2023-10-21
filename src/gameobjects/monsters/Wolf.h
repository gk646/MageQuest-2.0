#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_WOLF_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_WOLF_H_
struct Wolf final : public Monster {
  static constexpr int MAX_SCALED_LEVEL = 15;
  static constexpr uint8_t ATTACK_RANGE = 3;
  static constexpr uint8_t CHASE_RANGE = 10;
  static constexpr float ATTACK_CD = 120;
  static constexpr int base_health = 3;
  static constexpr float per_level = 2.5;
  static constexpr float base_speed = 2.4;
  Wolf(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, std::min(MAX_SCALED_LEVEL,level), per_level, base_speed},
                &textures::monsters::WOLF, MonsterType::WOLF, {45, 30}) {
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
        DrawTextureProFastEx(resource->walk[spriteCounter % 60 / 10],
                             pos.x_ + DRAW_X-2, pos.y_ + DRAW_Y-19, 2, 0, !flip, WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20], pos.x_ + DRAW_X-5,
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
        spriteCounter = 0;
        attack = 1;
      } else {
        attack_cd--;
      }
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X -5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 12,
                           pos.y_ + DRAW_Y - 19, 10, 0, !flip, WHITE);
    } else {
      attack = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_WOLF_H_
