#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERS_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERS_H_
struct Wolf final : public Monster {
  Wolf(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::WOLF, type,
                {45, 30}) {
    attackComponent.RegisterConeAttack(1, level, 90, 20, 30, resource->attackSounds[0], 1,
                                       40);
    attackComponent.RegisterConeAttack(1, level, 90, 20, 30, resource->attackSounds[1], 1,
                                       40);
    attackComponent.RegisterConeAttack(1, level, 90, 20, 30, resource->attackSounds[2], 1,
                                       40);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 60 / 10], pos.x_ + DRAW_X - 2,
                             pos.y_ + DRAW_Y - 19, 2, 0, !isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20], pos.x_ + DRAW_X - 5,
                             pos.y_ + DRAW_Y - 17, 5, 0, !isFlipped,
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
      attackComponent.Attack();
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 12,
                           pos.y_ + DRAW_Y - 19, 10, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
struct Snake final : public Monster {
  Snake(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SNAKE, type,
                {29, 19}) {
    attackComponent.RegisterConeAttack(
        1, level * 0.5F, 90, 15, 30, resource->attackSounds[0], 1, 35,
        {new Poison(level * 0.5F, 5 * 60, 60), nullptr, nullptr, nullptr});
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 60 / 15],
                             pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y - 30, 20, 0,
                             !isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20],
                             pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y - 30, 20, 0,
                             !isFlipped,
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
      attackComponent.Attack();
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 80 / 20;
    if (num < 3) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 20,
                           pos.y_ + DRAW_Y - 30, 20, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 90 / 15;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 20,
                           pos.y_ + DRAW_Y - 30, 20, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 220} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
struct BloodHound final : public Monster {
  BloodHound(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::BLOOD_HOUND,
                type, {45, 30}) {
    attackComponent.RegisterConeAttack(1, level, 90, 20, 30, resource->attackSounds[0],
                                       50);
    attackComponent.RegisterConeAttack(1, level, 90, 20, 30, resource->attackSounds[1],
                                       50);
    attackComponent.RegisterConeAttack(1, level, 90, 20, 30, resource->attackSounds[2],
                                       50);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 60 / 10], pos.x_ + DRAW_X - 2,
                             pos.y_ + DRAW_Y - 19, 2, 0, !isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20], pos.x_ + DRAW_X - 5,
                             pos.y_ + DRAW_Y - 17, 5, 0, !isFlipped,
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
      attackComponent.Attack();
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 105 / 15;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 12,
                           pos.y_ + DRAW_Y - 19, 10, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
struct FangShroom final : public Monster {
  FangShroom(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::MUSHROOM, type,
                {32, 39}) {
    attackComponent.RegisterConeAttack(1, level, 90, 25, 35, resource->attackSounds[0], 1,
                                       35);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 80 / 10],
                             pos.x_ + DRAW_X - 62, pos.y_ + DRAW_Y - 62, 3, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20],
                             pos.x_ + DRAW_X - 60, pos.y_ + DRAW_Y - 62, 3, 0, isFlipped,
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
      attackComponent.Attack();
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 100 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 60,
                           pos.y_ + DRAW_Y - 62, 3, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 81 / 9;
    if (num < 8) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 56,
                           pos.y_ + DRAW_Y - 62, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
struct SkeletonWarrior final : public Monster {
  SkeletonWarrior(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level,
                &textures::monsters::SKELETON_WARRIOR, type, {30, 48}) {
    attackComponent.RegisterConeAttack(1, stats.level, 90, 40, 48,
                                       resource->attackSounds[0], 32);
    attackComponent.RegisterConeAttack(2, stats.level, 90, 40, 48,
                                       resource->attackSounds[0], 16);
    attackComponent.RegisterConeAttack(3, stats.level, 120, 40, 48,
                                       resource->attackSounds[1], 16);
  }
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
    int num = spriteCounter % 96 / 16;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 12, -16, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 112 / 16;
    if (num < 6) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 20, -15, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = spriteCounter % 72 / 12;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 23,
                           pos.y_ + DRAW_Y - 13, -18, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
struct Ghost final : public Monster {
  static constexpr Vector2 directions[9] = {{-1, 0}, {1, 0},  {0, -1}, {0, 1},  {-1, 1},
                                            {1, 1},  {-1, 1}, {1, -1}, {-1, -1}};
  bool teleported = false;
  bool disappeared = true;
  Ghost(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::GHOST, type,
                {30, 40}) {}
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      DrawAttack1();
    } else if (actionState == 2) {
      DrawAppear();
    } else if (actionState == 3) {
      DrawDisappear();
    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 105 / 15], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 20, -3, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && attackComponent.globalCooldown == 0) {
      if (!disappeared) {
        actionState = 3;
        spriteCounter = 0;
      } else if (!teleported) {
        spriteCounter = 0;
        TeleportToTarget(target);
      }
    }
  }
  inline void TeleportToTarget(const Entity* ent) noexcept {
    int num = RANGE_100_FLOAT(RNG_ENGINE) - 1;
    num /= 10;

    for (int i = 0; i < 9; ++i) {
      int idx = (num + i) % 9;
      auto dir = directions[idx];

      float newX = ent->pos.x_ + 24 * dir.x;
      float newY = ent->pos.y_ + 24 * dir.y;

      if (!CheckTileCollision((newX + size.x / 2) / 48, (newY + size.y / 2) / 48)) {
        pos.x_ = newX;
        pos.y_ = newY;
        actionState = 2;
        teleported = true;
        break;
      }
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 70 / 10;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAppear() noexcept {
    int num = spriteCounter % 42 / 6;
    if (num < 6) {
      DrawTextureProFastEx(resource->walk[num], pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y, 0,
                           0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 1;
      teleported = false;
      attackComponent.ExecuteAttack(0);
      spriteCounter = 0;
      disappeared = false;
    }
  }
  inline void DrawDisappear() noexcept {
    int num = spriteCounter % 56 / 8;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
      disappeared = true;
    }
  }
  inline void DrawAttack1() noexcept {
    int num = spriteCounter % 100 / 20;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 14,
                           pos.y_ + DRAW_Y - 16, -2, 0, !isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
};
struct SkeletonSpear final : public Monster {
  SkeletonSpear(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SKELETON_SPEAR,
                type, {30, 50}) {
    attackComponent.RegisterConeAttack(1, stats.level, 120, 40, 48,
                                       resource->attackSounds[0], 15);
    attackComponent.RegisterConeAttack(2, stats.level, 120, 40, 48,
                                       resource->attackSounds[0], 15);
    attackComponent.RegisterConeAttack(3, stats.level, 120, 40, 48,
                                       resource->attackSounds[0], 24);
  }
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
        DrawTextureProFastEx(resource->walk[spriteCounter % 105 / 15],
                             pos.x_ + DRAW_X - 30, pos.y_ + DRAW_Y - 45, 0, 0,
                             pos.x_ + size.x / 2 > MIRROR_POINT, WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 84 / 12],
                             pos.x_ + DRAW_X - 38, pos.y_ + DRAW_Y - 45, 10, 0,
                             pos.x_ + size.x / 2 > MIRROR_POINT, WHITE);
      }
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 120 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 46, 0, 0, pos.x_ + size.x / 2 > MIRROR_POINT,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 18,
                           pos.y_ + DRAW_Y - 46, -30, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
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
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target)) {
      attackComponent.Attack();
    }
  }
};
struct SkeletonArcher final : public Monster {
  SkeletonArcher(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SKELETON_ARCHER,
                type, {30, 48}) {
    attackComponent.RegisterProjectileAttack(1, (float)level, 80, ARROW_NORMAL);
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
    if (target && WalkCloseToEntity(target, attackComponent.attackRangeTiles)) {
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
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERS_H_
