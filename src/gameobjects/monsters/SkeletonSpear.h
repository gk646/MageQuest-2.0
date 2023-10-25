#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
struct SkeletonSpear final : public Monster {
  SkeletonSpear(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SKELETON_SPEAR,
                type, {30, 50}) {}
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else if (actionState == 2) {
      draw_attack2();
    } else if (actionState == 3) {
      draw_attack3();
    } else {
      if (isMoving) {
        draw_walk();
      } else {
        draw_idle();
      }
    }
    health_bar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 120 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 46, 0, 0, pos.x_ + size.x / 2 > MIRROR_POINT,
                           WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 18,
                           pos.y_ + DRAW_Y - 46, -30, 0, isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 46, -10, 0,
                           pos.x_ + size.x / 2 > MIRROR_POINT, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = spriteCounter % 72 / 12;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 16,
                           pos.y_ + DRAW_Y - 46, -32, 0,
                           pos.x_ + size.x / 2 > MIRROR_POINT, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_walk() noexcept {
    if (pos.x_ + size.x / 2 > MIRROR_POINT) {
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
    if (pos.x_ + size.x / 2 > MIRROR_POINT) {
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
      if (AttackPlayer3Attacks()) {
        if (actionState == 1) {
          PROJECTILES.emplace_back(new AttackCone(GetAttackConeBounds(40, 48), false, 120,
                                                  15, stats.level, {},
                                                  resource->attack_sound[0], this));
        } else if (actionState == 2) {
          PROJECTILES.emplace_back(new AttackCone(GetAttackConeBounds(40, 48), false, 120,
                                                  15, stats.level, {},
                                                  resource->attack_sound[0], this));
        } else {
          PROJECTILES.emplace_back(new AttackCone(GetAttackConeBounds(40, 48), false, 120,
                                                  24, stats.level, {},
                                                  resource->attack_sound[0], this));
        }
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
