#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_

struct SkeletonWarrior final : public Monster {
  static constexpr int base_health = 7;
  static constexpr int per_level = 5;
  static constexpr float base_speed = 1.5;
  SkeletonWarrior(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, per_level, base_speed},
                &textures::SKELETON_WARRIOR, {30, 47}) {
    attack_cd = 50;
  }

  void draw() final {
    if (attack == -100) [[unlikely]] {
      draw_death();
    } else if (attack == 1) {
      draw_attack1();
    } else if (attack == 2) {
      draw_attack2();
    } else if (attack == 3) {
      draw_attack3();
    } else {
      if (moving) {
        draw_walk();
      } else {
        draw_idle();
      }
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() final {
    Monster::update();
    if (move_to_player() && attack == 0 && attack_cd < 0) {
      int num = RANGE_100(RNG_ENGINE);
      attack_cd = 160;
      sprite_counter = 0;
      if (num < 33) {
        attack = 1;
      } else if (num < 66) {
        attack = 2;
      } else {
        attack = 3;
      }
    } else {
      attack_cd--;
    }
  }
  inline void draw_death() noexcept {
    int num = sprite_counter % 175 / 35;
    if (num < 4) {
      if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
        auto texture = resource->death[num];
        DrawTexturePro(
            texture,
            {0, 0, static_cast<float>(-texture.width),
             static_cast<float>(texture.height)},
            {pos.x_ + DRAW_X, pos.y_ + DRAW_Y - 46, static_cast<float>(texture.width),
             static_cast<float>(texture.height)},
            {0, 0}, 0, WHITE);
      } else {

        DrawTextureProFast(resource->death[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 46, 0, WHITE);
      }
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = sprite_counter % 96 / 16;
    if (num < 5) {
      if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
        auto texture = resource->attack1[num];
        DrawTexturePro(
            texture,
            {0, 0, static_cast<float>(-texture.width),
             static_cast<float>(texture.height)},
            {pos.x_ + DRAW_X - 40, pos.y_ + DRAW_Y - 13,
             static_cast<float>(texture.width), static_cast<float>(texture.height)},
            {0, 0}, 0, WHITE);
      } else {
        DrawTextureProFast(resource->attack1[num], pos.x_ + DRAW_X - 23,
                           pos.y_ + DRAW_Y - 13, 0, WHITE);
      }
    } else {
      attack = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = sprite_counter % 112 / 16;
    if (num < 6) {
      if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
        auto texture = resource->attack2[num];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x_ + DRAW_X - 37, pos.y_ + DRAW_Y - 20,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        DrawTextureProFast(resource->attack2[num], pos.x_ + DRAW_X - 32,
                           pos.y_ + DRAW_Y - 20, 0, WHITE);
      }
    } else {
      attack = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = sprite_counter % 72 / 12;
    if (num < 4) {
      if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
        auto texture = resource->attack3[num];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x_ + DRAW_X - 40, pos.y_ + DRAW_Y - 13,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        DrawTextureProFast(resource->attack3[num], pos.x_ + DRAW_X - 23,
                           pos.y_ + DRAW_Y - 13, 0, WHITE);
      }
    } else {
      attack = 0;
    }
  }
  inline void draw_walk() noexcept {
    if (pos.x_ + size.x_ / 2 > MIRROR_POINT) {
      auto texture = resource->walk[sprite_counter % 140 / 20];
      DrawTexturePro(
          texture,
          {0, 0, static_cast<float>(-texture.width), static_cast<float>(texture.height)},
          {pos.x_ + DRAW_X - 40, pos.y_ + DRAW_Y - 5, static_cast<float>(texture.width),
           static_cast<float>(texture.height)},
          {0, 0}, 0, WHITE);
    } else {
      DrawTextureProFast(resource->walk[sprite_counter % 140 / 20], pos.x_ + DRAW_X - 35,
                         pos.y_ + DRAW_Y - 5, 0, WHITE);
    }
  }
  inline void draw_idle() noexcept {
    DrawTextureProFastEx(resource->idle[sprite_counter % 112 / 16], pos.x_ + DRAW_X,
                         pos.y_ + DRAW_Y, -10, 0, pos.x_ + size.x_ / 2 > MIRROR_POINT,
                         WHITE);
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
