#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERS_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERS_H_
struct Wolf final : public Monster {
  Wolf(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::WOLF, type,
                {45, 30}) {
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 20, 30,
                                       resource->attackSounds[0], 1, 40);
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 20, 30,
                                       resource->attackSounds[1], 1, 40);
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 20, 30,
                                       resource->attackSounds[2], 1, 40);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
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
      attackComponent.AttackClose();
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
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
        1, stats.effects[WEAPON_DAMAGE], monsterIdToScaler[type].attackCD, 15, 30,
        resource->attackSounds[0], 1, 35,
        {new Poison(stats.effects[WEAPON_DAMAGE], 5 * 60, 60), nullptr, nullptr,
         nullptr});
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
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
      attackComponent.AttackClose();
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 80 / 20;
    if (num < 3) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 20,
                           pos.y_ + DRAW_Y - 30, 20, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
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
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 20, 30,
                                       resource->attackSounds[0], 50);
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 20, 30,
                                       resource->attackSounds[1], 50);
    attackComponent.RegisterConeAttack(
        1, stats.effects[WEAPON_DAMAGE], 90 + 105, 20, 30, resource->attackSounds[2], 50,
        0, {new Bleed(stats.effects[WEAPON_DAMAGE] * 0.5F, 300, 60)});
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
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
      attackComponent.AttackClose();
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
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
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 22, 33,
                                       resource->attackSounds[0], 1, 35);
    attackComponent.RegisterConeAttack(
        2, stats.effects[WEAPON_DAMAGE], monsterIdToScaler[type].attackCD, 25, 35,
        resource->attackSounds[0], 1, 50,
        {new Poison(stats.effects[WEAPON_DAMAGE] * 0.4F, 300, 60)}, 0.4F);
    attackComponent.RegisterAbility(
        3, 10 * 60,
        [](Monster* attacker) {
          Projectile::ShootRadial(
              attacker->GetMiddlePoint(), 4, SPORE_SHOT,
              ((FangShroom*)attacker)->stats.effects[WEAPON_DAMAGE] * 0.6F, true);
        },
        3, 50, 0.5F);
  }

  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
    } else if (actionState == 2) {
      DrawBite();
    } else if (actionState == 3) {
      DrawRangedAttack();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 80 / 10],
                             pos.x_ + DRAW_X - 64, pos.y_ + DRAW_Y - 64, 0, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20],
                             pos.x_ + DRAW_X - 64, pos.y_ + DRAW_Y - 64, 0, 0, isFlipped,
                             hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
      }
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target) {
      attackComponent.AttackFar();
      if (WalkToEntity(target)) {
        attackComponent.AttackClose();
      }
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 100 / 20;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 64,
                           pos.y_ + DRAW_Y - 64, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
    int num = spriteCounter % 63 / 7;
    if (num < 8) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 64,
                           pos.y_ + DRAW_Y - 64, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void DrawBite() noexcept {
    int num = spriteCounter % 63 / 7;
    if (num < 8) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 64,
                           pos.y_ + DRAW_Y - 64, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void DrawRangedAttack() noexcept {
    int num = spriteCounter % 78 / 7;
    if (num < 11) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 64,
                           pos.y_ + DRAW_Y - 64, 0, 0, isFlipped,
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
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 40, 48,
                                       resource->attackSounds[0], 32);
    attackComponent.RegisterConeAttack(2, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 40, 48,
                                       resource->attackSounds[0], 16);
    attackComponent.RegisterConeAttack(
        3, stats.effects[WEAPON_DAMAGE] * 1.2F, 350, 40, 48, resource->attackSounds[1],
        16, 0, {new Bleed(stats.effects[WEAPON_DAMAGE] * 0.2F, 300, 60)});
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
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
      attackComponent.AttackClose();
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 175 / 35;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 48, -20, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
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
      DrawDeath();
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
    if (target && attackComponent.currentCooldown == 0) {
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
  inline void DrawDeath() noexcept {
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
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 40, 48,
                                       resource->attackSounds[0], 15);
    attackComponent.RegisterConeAttack(2, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 40, 48,
                                       resource->attackSounds[0], 15);
    attackComponent.RegisterConeAttack(
        3, stats.effects[WEAPON_DAMAGE] * 1.2F, 370, 40, 48, resource->attackSounds[0],
        24, 0, {new Bleed(stats.effects[WEAPON_DAMAGE], 300, 60)});
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
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
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 120 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 22,
                           pos.y_ + DRAW_Y - 46, 0, 0, pos.x_ + size.x / 2 > MIRROR_POINT,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
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
      attackComponent.AttackClose();
    }
  }
};
struct SkeletonArcher final : public Monster {
  SkeletonArcher(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SKELETON_ARCHER,
                type, {30, 48}) {
    attackComponent.RegisterProjectileAttack(1, stats.effects[WEAPON_DAMAGE],
                                             monsterIdToScaler[type].attackCD,
                                             ARROW_NORMAL, 1, 50);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
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
    if (target) {
      attackComponent.AttackFar();
      if (WalkCloseToEntity(target, attackComponent.attackRangeTiles)) {
        attackComponent.AttackClose();
      }
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 175 / 35;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 27,
                           pos.y_ + DRAW_Y - 48, -20, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
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
struct SkullWolf final : public Monster {
  unsigned char alpha = 255;
  SkullWolf(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::SKULL_WOLF,
                type, {50, 27}) {
    attackComponent.RegisterConeAttack(1, stats.effects[WEAPON_DAMAGE],
                                       monsterIdToScaler[type].attackCD, 20, 30,
                                       resource->attackSounds[0], 25);
  }
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawAttack1();
    } else if (actionState == 2) {
      DrawDisappear();
    } else if (actionState == 3) {
      DrawAppear();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(
            resource->walk[spriteCounter % 35 / 7], pos.x_ + DRAW_X - 5,
            pos.y_ + DRAW_Y - 20, -3, 0, !isFlipped,
            hitFlashDuration > 0 ? Color{255, 0, 68, 200} : Color{255, 255, 255, alpha});
      } else {
        DrawTextureProFastEx(
            resource->idle[spriteCounter % 60 / 10], pos.x_ + DRAW_X - 5,
            pos.y_ + DRAW_Y - 20, -3, 0, !isFlipped,
            hitFlashDuration > 0 ? Color{255, 0, 68, 200} : Color{255, 255, 255, alpha});
      }
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE()
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target)) {
      if (attackComponent.currentCooldown == 0) {
        if (alpha == 255 && RANGE_100_FLOAT(RNG_ENGINE) < 25) {
          actionState = 2;
          spriteCounter = 0;
          attackComponent.currentCooldown = 60;
          effectHandler.AddEffect(new Swiftness(25, 700));
          effectHandler.AddEffect(new Berserk(1.4F, 700));
          PlaySoundR(resource->attackSounds[1]);
        } else if (alpha == 50 && !effectHandler.IsEffectActive(EffectType::SWIFTNESS)) {
          actionState = 3;
          spriteCounter = 0;
          attackComponent.currentCooldown = 60;
        }
      }
      attackComponent.AttackClose();
    }
  }
  inline void DrawDeath() noexcept {
    int num = spriteCounter % 80 / 10;
    if (num < 7) {
      DrawTextureProFastEx(
          resource->death[num], pos.x_ + DRAW_X - 5, pos.y_ + DRAW_Y - 20, -3, 0,
          !isFlipped,
          hitFlashDuration > 0 ? Color{255, 0, 68, 200} : Color{255, 255, 255, alpha});
    } else {
      isDead = true;
    }
  }
  inline void DrawAttack1() noexcept {
    int num = spriteCounter % 35 / 7;
    if (spriteCounter < 35) {
      DrawTextureProFastEx(
          resource->walk[num], pos.x_ + DRAW_X - 5, pos.y_ + DRAW_Y - 20, -3, 0,
          !isFlipped,
          hitFlashDuration > 0 ? Color{255, 0, 68, 200} : Color{255, 255, 255, alpha});
    } else {
      actionState = 0;
    }
  }
  inline void DrawDisappear() noexcept {
    int num = spriteCounter % 60 / 15;
    if (spriteCounter < 60) {
      DrawTextureProFastEx(
          resource->attack2[num], pos.x_ + DRAW_X - 5, pos.y_ + DRAW_Y - 20, -3, 0,
          !isFlipped,
          hitFlashDuration > 0 ? Color{255, 0, 68, 200} : Color{255, 255, 255, alpha});
    } else {
      alpha = 50;
      actionState = 0;
    }
  }
  inline void DrawAppear() noexcept {
    int num = spriteCounter % 60 / 15;
    if (spriteCounter < 60) {
      DrawTextureProFastEx(
          resource->special[num], pos.x_ + DRAW_X - 5, pos.y_ + DRAW_Y - 20, -3, 0,
          !isFlipped,
          hitFlashDuration > 0 ? Color{255, 0, 68, 200} : Color{255, 255, 255, alpha});
    } else {
      alpha = 255;
      actionState = 0;
    }
  }
};
struct BossStoneGolem final : public Monster {
  bool shouldUseLaserAttack = false;
  HealthDropComponent healthDrop{0.75F, 0.5F, 0.25F, 0};
  BossStoneGolem(const Point& pos, int level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level,
                &textures::monsters::BOSS_STONE_GOLEM, type, {99, 91}) {
    attackComponent.RegisterConeAttack(3, stats.effects[WEAPON_DAMAGE], 140, 40, 91,
                                       sound::EMPTY_SOUND, 50, 0, {new Slow(20, 120)});
    attackComponent.RegisterAbility(
        2, 25 * 60,
        [](Monster* attacker) {
          attacker->effectHandler.AddEffect(new Resistance(100, 150));
          attacker->attackComponent.currentCooldown = 150;
        },
        0);
    attackComponent.RegisterAbility(
        5, 15 * 60,
        [](Monster* attacker) {
          attacker->effectHandler.AddEffect(new Resistance(20, 600));
        },
        0, 0, 0.75F);
    attackComponent.RegisterAbility(
        6, 15 * 60,
        [](Monster* attacker) {
          attacker->effectHandler.AddEffect(new Berserk(1.2F, 600));
        },
        0, 0, 0.75F);
    attackComponent.RegisterProjectileAttack(1, stats.effects[WEAPON_DAMAGE], 150,
                                             ARROW_NORMAL, 3, 1, {}, 0.75F);
  }

  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      DrawDeath();
    } else if (actionState == 1) {
      DrawRangeAttack();
    } else if (actionState == 2) {
      DrawImmune();
    } else if (actionState == 3) {
      DrawMelee();
    } else if (actionState == 4) {
      DrawLaserAttack();
    } else if (actionState == 5) {
      DrawArmourBuff();
    } else if (actionState == 6) {
      DrawGlow();
    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 60 / 15], pos.x_ + DRAW_X - 50,
                           pos.y_ + DRAW_Y - 48, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  inline void Update() final {
    MONSTER_UPDATE()
    auto target = threatManager.GetHighestThreatTarget();
    if (target) {
      attackComponent.AttackFar();
      healthDrop.Update(shouldUseLaserAttack, stats);
      if (shouldUseLaserAttack) {
        shouldUseLaserAttack = false;
        actionState = 4;
        attackComponent.currentCooldown = 600;
      }
      if (WalkToEntity(target)) {
        if (attackComponent.currentCooldown == 0) {}
        attackComponent.AttackClose();
      }
    }
  }

  inline void DrawDeath() noexcept {
    int num = spriteCounter % 150 / 10;
    if (num < 14) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, 0,
                           isFlipped, WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawGlow() noexcept {
    int num = spriteCounter % 90 / 10;
    if (num < 8) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 46,
                           pos.y_ + DRAW_Y - 28, -10, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void DrawMelee() noexcept {
    int num = spriteCounter % 80 / 10;
    if (num < 7) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 50,
                           pos.y_ + DRAW_Y - 38, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void DrawRangeAttack() noexcept {
    int num = spriteCounter % 80 / 8;
    if (num < 9) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 50,
                           pos.y_ + DRAW_Y - 35, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void DrawImmune() noexcept {
    int num = spriteCounter % 80 / 10;
    if (spriteCounter > 70) num = 7;
    DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 47,
                         pos.y_ + DRAW_Y - 43, 0, 0, isFlipped,
                         hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    if (attackComponent.currentCooldown == 0) {
      actionState = 0;
    }
  }
  inline void DrawLaserAttack() noexcept {
    int num = spriteCounter % 70 / 10;
    if (spriteCounter > 60) num = 6;
    DrawTextureProFastEx(resource->attack4[num], pos.x_ + DRAW_X - 50,
                         pos.y_ + DRAW_Y - 55, 0, 0, isFlipped,
                         hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    if (attackComponent.currentCooldown == 0) {
      actionState = 0;
    }
  }
  inline void DrawArmourBuff() noexcept {
    int num = spriteCounter % 121 / 12;
    if (num < 10) {
      DrawTextureProFastEx(resource->attack5[num], pos.x_ + DRAW_X - 50,
                           pos.y_ + DRAW_Y - 57, 0, 0, isFlipped,
                           hitFlashDuration > 0 ? Color{255, 0, 68, 200} : WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERS_H_
