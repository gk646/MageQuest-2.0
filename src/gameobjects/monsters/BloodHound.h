#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_BLOODHOUND_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_BLOODHOUND_H_
struct BloodHound final : public Monster {

  BloodHound(const Point& pos, uint8_t level, MonsterType type) noexcept
      : Monster(pos, monsterIdToScaler[type], level, &textures::monsters::BLOOD_HOUND,
                type, {45, 30}) {
    attackComponent.RegisterConeAttack(1, level, 90, 30, 30, sound::EMPTY_SOUND, 10);
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
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_BLOODHOUND_H_
