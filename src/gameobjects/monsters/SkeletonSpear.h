#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
struct SkeletonSpear final : public Monster {
  static constexpr int base_health = 5;
  static constexpr float per_level = 3.5;
  static constexpr float base_speed = 2;
  SkeletonSpear(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, base_speed},
                &textures::monsters::SKELETON_SPEAR, MonsterType::SKEL_SPEAR, {30, 50}) {
    attack_cd = 120;
  }

  void draw() final {
    if (attack == -100) [[unlikely]] {
      draw_death();
    } else if (attack == 1) {
      draw_attack1();
    } else if (attack == 2) {
      draw_attack2();
    } else if (attack == 3) {
      draw_attack3();
    } else {
      if (moving) {
        draw_walk();
      } else {
        draw_idle();
      }
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 120 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 46, 0, 0,
                           pos.x_ + size.x_ / 2 > MIRROR_POINT, WHITE);
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 18,
                           pos.y_ + DRAW_Y - 46, -30, 0,
                           pos.x_ + size.x_ / 2 > MIRROR_POINT, WHITE);
    } else {
      attack = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 46, -10, 0,
                           pos.x_ + size.x_ / 2 > MIRROR_POINT, WHITE);
    } else {
      attack = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = spriteCounter % 72 / 12;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 16,
                           pos.y_ + DRAW_Y - 46, -32, 0,
                           pos.x_ + size.x_ / 2 > MIRROR_POINT, WHITE);
    } else {
      attack = 0;
    }
  }
  inline void draw_walk() noexcept {
    if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
      auto texture = resource->walk[spriteCounter % 105 / 15];
      DrawTexturePro(
          texture,
          {0, 0, static_cast<float>(-texture.width), static_cast<float>(texture.height)},
          {pos.x_ + DRAW_X - 30, pos.y_ + DRAW_Y - 45, static_cast<float>(texture.width),
           static_cast<float>(texture.height)},
          {0, 0}, 0, WHITE);
    } else {
      DrawTextureProFast(resource->walk[spriteCounter % 105 / 15], pos.x_ + DRAW_X - 30,
                         pos.y_ + DRAW_Y - 45, 0, WHITE);
    }
  }
  inline void draw_idle() noexcept {
    if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
      auto texture = resource->idle[spriteCounter % 84 / 12];
      DrawTexturePro(
          texture,
          {0, 0, static_cast<float>(-texture.width), static_cast<float>(texture.height)},
          {pos.x_ + DRAW_X - 38, pos.y_ + DRAW_Y - 45, static_cast<float>(texture.width),
           static_cast<float>(texture.height)},
          {0, 0}, 0, WHITE);
    } else {
      DrawTextureProFast(resource->idle[spriteCounter % 84 / 12], pos.x_ + DRAW_X - 28,
                         pos.y_ + DRAW_Y - 45, 0, WHITE);
    }
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target)) {
      AttackPlayer3Attacks();
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
