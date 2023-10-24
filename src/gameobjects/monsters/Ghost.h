#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
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
                           pos.y_ + DRAW_Y - 20, -3, 0, !isFlipped, WHITE);
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && attackStats.IsAttackReady(actionState)) {
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
                           pos.y_ + DRAW_Y - 15, 0, 0, !isFlipped, WHITE);
    } else {
      isDead = true;
    }
  }
  inline void DrawAppear() noexcept {
    int num = spriteCounter % 42 / 6;
    if (num < 6) {
      DrawTextureProFastEx(resource->walk[num], pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y, 0,
                           0, !isFlipped, WHITE);
    } else {
      actionState = 1;
      teleported = false;
      PROJECTILES.push_back(new PsychicScream(pos, false, 5, HitType::CONTINUOUS));
      spriteCounter = 0;
      disappeared = false;
      attackStats.ResetCooldown();
    }
  }
  inline void DrawDisappear() noexcept {
    int num = spriteCounter % 56 / 8;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !isFlipped, WHITE);
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
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
