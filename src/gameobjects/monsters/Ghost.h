#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
struct Ghost final : public Monster {
  static constexpr int base_health = 4;
  static constexpr float per_level = 3.5;
  static constexpr float ATTACK_CD = 120;
  static constexpr uint8_t ATTACK_RANGE = 6;
  static constexpr uint8_t CHASE_RANGE = 10;
  static constexpr Vector2 directions[9] = {{-1, 0}, {1, 0},  {0, -1}, {0, 1},  {-1, 1},
                                            {1, 1},  {-1, 1}, {1, -1}, {-1, -1}};
  bool teleported = false;
  bool disappeared = true;
  Ghost(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, 0},
                &textures::monsters::GHOST, MonsterType::WOLF, {30, 40}) {
    attack_cd = ATTACK_CD;
    AttackRange = ATTACK_RANGE;
    ChaseRange = CHASE_RANGE;
  }
  void Draw() final {
    if (attack == -100) [[unlikely]] {
      draw_death();
    } else if (attack == 1) {
      DrawAttack1();
    } else if (attack == 2) {
      DrawAppear();
    } else if (attack == 3) {
      DrawDisappear();
    } else {
      DrawTextureProFastEx(resource->idle[spriteCounter % 105 / 15], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 20, -3, 0, !flip, WHITE);
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && attack_cd <= 0 && attack == 0) {
      if (!disappeared) {
        attack = 3;
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

      if (!CheckTileCollision((newX + size.x_ / 2) / 48, (newY + size.y_ / 2) / 48)) {
        pos.x_ = newX;
        pos.y_ = newY;
        attack = 2;
        teleported = true;
        break;
      }
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 70 / 10;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void DrawAppear() noexcept {
    int num = spriteCounter % 56 / 8;
    if (num < 6) {
      DrawTextureProFastEx(resource->walk[num], pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y, 0,
                           0, !flip, WHITE);
    } else {
      attack = 1;
      teleported = false;
      PROJECTILES.push_back(new PsychicScream(pos, false, 5, HitType::CONTINUOUS));
      spriteCounter = 0;
      disappeared = false;
      attack_cd = ATTACK_CD;
    }
  }
  inline void DrawDisappear() noexcept {
    int num = spriteCounter % 56 / 8;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !flip, WHITE);
    } else {
      attack = 0;
      disappeared = true;
    }
  }
  inline void DrawAttack1() noexcept {
    int num = spriteCounter % 100 / 20;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 14,
                           pos.y_ + DRAW_Y - 16, -2, 0, !flip, WHITE);
    } else {
      attack = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
