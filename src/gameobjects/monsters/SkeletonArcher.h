#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONARCHER_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONARCHER_H_
struct SkeletonArcher final : public Monster {
  SkeletonArcher(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SKELETON_ARCHER,
                type, {30, 48}) {
    attackComponent.RegisterProjectileAttack(1,level, 80, ARROW_NORMAL);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 160 / 20],
                             pos.x_ + DRAW_X - 33, pos.y_ + DRAW_Y - 6, 0, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 112 / 16],
                             pos.x_ + DRAW_X - 30, pos.y_ + DRAW_Y - 2, -6, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      }
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkCloseToEntity(target, attackComponent.attackRange)) {
      attackComponent.Attack();
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 175 / 35;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 48, -20, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 80 / 5;
    if (num < 15) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 30,
                           pos.y_ + DRAW_Y - 5, -16, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONARCHER_H_
