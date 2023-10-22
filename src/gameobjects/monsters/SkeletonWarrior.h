#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_

struct SkeletonWarrior final : public Monster {
  static constexpr int base_health = 7;
  static constexpr int per_level = 5;
  static constexpr float base_speed = 1.5;
  SkeletonWarrior(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, base_speed},
                &textures::monsters::SKELETON_WARRIOR, MonsterType::SKEL_WAR, {30, 48}) {
    attack_cd = 50;
  }
  void Draw() final {
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
        DrawTextureProFastEx(resource->walk[spriteCounter % 140 / 20],
                             pos.x_ + DRAW_X - 33, pos.y_ + DRAW_Y - 6, 0, 0, flip,
                             WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 112 / 16],
                             pos.x_ + DRAW_X - 30, pos.y_ + DRAW_Y - 2, -10, 0, flip,
                             WHITE);
      }
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target)) {
      if (AttackPlayer3Attacks()) {
        Point attackPoint = {flip ? pos.x_ - 60 : pos.x_ + 30, pos.y_};
        Point attackSize = {flip ? pos.x_ - 60 : pos.x_ + 30, pos.y_};
        if (attack == 1) {
          PROJECTILES.emplace_back(new AttackCone(
              attackPoint, {40, 50}, false, 120, 30,
              stats.level, {}, resource->attack_sound[0], this));
        } else if (attack == 2) {
          PROJECTILES.emplace_back(new AttackCone(attackPoint, {40, 50}, false, 120, 30,
                                                  stats.level, {},
                                                  resource->attack_sound[1], this));
        } else {
          PROJECTILES.emplace_back(new AttackCone(attackPoint, {40, 50}, false, 120, 30,
                                                  stats.level, {},
                                                  resource->attack_sound[0], this));
        }
      }
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 175 / 35;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 48, -20, 0, flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 96 / 16;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 12, -16, 0, flip, WHITE);
    } else {
      attack = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 112 / 16;
    if (num < 6) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 20, -15, 0, flip, WHITE);
    } else {
      attack = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = spriteCounter % 72 / 12;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 23,
                           pos.y_ + DRAW_Y - 13, -18, 0, flip, WHITE);
    } else {
      attack = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
