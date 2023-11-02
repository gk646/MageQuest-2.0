#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_

struct SkeletonWarrior final : public Monster {
  SkeletonWarrior(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level,
                &textures::monsters::SKELETON_WARRIOR, type, {30, 48}) {}
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
        DrawTextureProFastEx(resource->walk[spriteCounter % 140 / 20],
                             pos.x_ + DRAW_X - 33, pos.y_ + DRAW_Y - 6, 0, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 112 / 16],
                             pos.x_ + DRAW_X - 30, pos.y_ + DRAW_Y - 2, -10, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      }
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target)) {
      if (AttackPlayer3Attacks()) {
        if (actionState == 1) {
          PROJECTILES.emplace_back(new AttackCone(GetAttackConeBounds(40, 48), false, 120,
                                                  32, stats.level, {},
                                                  resource->attack_sound[0], this));
        } else if (actionState == 2) {
          PROJECTILES.emplace_back(new AttackCone(GetAttackConeBounds(40, 48), false, 120,
                                                  16, stats.level, {},
                                                  resource->attack_sound[1], this));
        } else {
          PROJECTILES.emplace_back(new AttackCone(GetAttackConeBounds(40, 48), false, 120,
                                                  16, stats.level, {},
                                                  resource->attack_sound[0], this));
        }
      }
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 175 / 35;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 48, -20, 0, isFlipped, hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 96 / 16;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 12, -16, 0, isFlipped, hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 112 / 16;
    if (num < 6) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 20, -15, 0, isFlipped, hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = spriteCounter % 72 / 12;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 23,
                           pos.y_ + DRAW_Y - 13, -18, 0, isFlipped, hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
