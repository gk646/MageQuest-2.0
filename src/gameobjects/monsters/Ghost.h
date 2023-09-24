#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
struct Ghost final : public Monster {
  static constexpr int base_health = 4;
  static constexpr float per_level = 3.5;
  static constexpr float base_speed = 2.2;
  static constexpr float ATTACK_CD = 120;
  bool teleported = false;
  bool disappeared = true;
  Ghost(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, base_speed},
                &textures::monsters::GHOST, MonsterType::WOLF, {30, 40}) {
    attack_cd = 50;
  }
  void draw() final {
    if (attack == -100) [[unlikely]] {
      draw_death();
    } else if (attack == 1) {
      DrawAttack1();
    } else if (attack == 2) {
      DrawAppear();
    } else if (attack == 3) {
      DrawDisappear();
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
    auto target = threatManager.GetHighestThreatTarget();
    if (target && attack_cd <= 0 && attack == 0) {
      if (!disappeared) {
        attack = 3;
        sprite_counter = 0;
      } else if (!teleported) {
        sprite_counter = 0;
        TeleportToTarget(target);
      }
    }
  }
  inline void TeleportToTarget(const Entity* ent) noexcept {
    if (!CheckTileCollision(ent->tile_pos.x - 1, ent->tile_pos.y)) {
      pos.x_ = (ent->tile_pos.x - 1) * 48 + 24;
      pos.y_ = ent->tile_pos.y * 48;
      attack = 2;
      teleported = true;
    } else if (!CheckTileCollision(ent->tile_pos.x + 1, ent->tile_pos.y)) {
      pos.x_ = (ent->tile_pos.x + 1) * 48 - 24;
      pos.y_ = ent->tile_pos.y * 48;
      attack = 2;
      teleported = true;
    } else if (!CheckTileCollision(ent->tile_pos.x, ent->tile_pos.y - 1)) {
      pos.x_ = ent->tile_pos.x * 48;
      pos.y_ = (ent->tile_pos.y - 1) * 48 + 24;
      attack = 2;
      teleported = true;
    } else if (!CheckTileCollision(ent->tile_pos.x, ent->tile_pos.y + 1)) {
      pos.x_ = ent->tile_pos.x * 48;
      pos.y_ = (ent->tile_pos.y + 1) * 48 - 24;
      attack = 2;
      teleported = true;
    }
  }
  inline void draw_death() noexcept {
    int num = sprite_counter % 70 / 10;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void DrawAppear() noexcept {
    int num = sprite_counter % 56 / 8;
    if (num < 6) {
      DrawTextureProFastEx(resource->walk[num], pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y, 0,
                           0, !flip, WHITE);
    } else {
      attack = 1;
      teleported = false;
      PROJECTILES.push_back(new PsychicScream(pos, false, 5, HitType::CONTINUOUS, {}));
      sprite_counter = 0;
      disappeared = false;
      attack_cd = ATTACK_CD;
    }
  }
  inline void DrawDisappear() noexcept {
    int num = sprite_counter % 56 / 8;
    if (num < 6) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X - 17,
                           pos.y_ + DRAW_Y - 15, 0, 0, !flip, WHITE);
    } else {
      attack = 0;
      disappeared = true;
    }
  }
  inline void DrawAttack1() noexcept {
    int num = sprite_counter % 100 / 20;
    if (num < 4) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 14,
                           pos.y_ + DRAW_Y - 16, -2, 0, !flip, WHITE);
    } else {
      attack = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_GHOST_H_
