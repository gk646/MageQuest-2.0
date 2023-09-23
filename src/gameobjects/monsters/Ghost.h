#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
struct Ghost final : public Monster {
  static constexpr int base_health = 4;
  static constexpr float per_level = 3.5;
  static constexpr float base_speed = 2.2;
  static constexpr float ATTACK_CD = 120;
  Ghost(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, base_speed},
                &textures::monsters::GHOST, MonsterType::WOLF, {30, 40}) {
    attack_cd = 50;
  }
  void draw() final {
    if (attack == -100) [[unlikely]] {
      draw_death();
    } else if (attack == 1) {
      draw_attack1();
    } else {
      DrawTextureProFastEx(resource->idle[sprite_counter % 105 / 15],
                           pos.x_ + DRAW_X - 17, pos.y_ + DRAW_Y - 20, -3, 0, !flip,
                           WHITE);
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() final {
    BASIC_UPDATE();
    flip = pos.x_ + size.x_ / 2 > MIRROR_POINT;
  }
  void GhostAttackScript() {}

  inline void draw_death() noexcept {
    int num = sprite_counter % 140 / 20;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, 0,
                           !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void DrawAppear() noexcept {
    int num = sprite_counter % 140 / 20;
    if (num < 6) {
      DrawTextureProFastEx(resource->walk[num], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, 0,
                           !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void DrawDisappear() noexcept {
    int num = sprite_counter % 140 / 20;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, 0,
                           !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = sprite_counter % 100 / 20;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 12,
                           pos.y_ + DRAW_Y - 19, 10, 0, !flip, WHITE);
    } else {
      attack = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
