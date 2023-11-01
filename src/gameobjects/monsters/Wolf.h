#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_WOLF_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_WOLF_H_
struct Wolf final : public Monster {
  Wolf(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::WOLF, type,
                {45, 30}) {}
  void Draw() final {
    if (actionState == -100) [[unlikely]] {
      draw_death();
    } else if (actionState == 1) {
      draw_attack1();
    } else {
      if (isMoving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 60 / 10], pos.x_ + DRAW_X - 2,
                             pos.y_ + DRAW_Y - 19, 2, 0, !isFlipped, WHITE);
      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 20], pos.x_ + DRAW_X - 5,
                             pos.y_ + DRAW_Y - 17, 5, 0, !isFlipped, WHITE);
      }
    }
    healthBar.Draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    DRAW_HITBOXES();
  }
  void Update() final {
    MONSTER_UPDATE();
    auto target = threatManager.GetHighestThreatTarget();
    if (target && WalkToEntity(target) && attackStats.IsAttackReady(actionState)) {
      attackStats.ResetCooldown();
      spriteCounter = 0;
      actionState = 1;
    }
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 5,
                           pos.y_ + DRAW_Y - 19, 3, 0, !isFlipped, WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 140 / 20;
    if (num < 5) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 12,
                           pos.y_ + DRAW_Y - 19, 10, 0, !isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_WOLF_H_
