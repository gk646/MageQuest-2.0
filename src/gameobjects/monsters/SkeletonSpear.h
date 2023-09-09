#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
struct SkeletonSpear final : public Monster {
  static constexpr int base_health = 10;
  static constexpr float base_speed = 2.2;
  SkeletonSpear(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level,base_speed}, &textures::SKELETON_SPEAR,
                {30, 50}) {
  }

  void draw() final {
    if (attack == -100) {
      int num = sprite_counter % 120 / 20;
      if (num < 5) {
        DrawTextureProFast(resource->death[num], pos.x() + DRAW_X - 28,
                           pos.y() + DRAW_Y - 46, 0, WHITE);
      } else {
        dead = true;
      }
    } else if (attack == 1) {
      int num = sprite_counter % 75 / 15;
      if (num < 4) {
        DrawTextureProFast(resource->attack1[num], pos.x() + DRAW_X - 20,
                           pos.y() + DRAW_Y - 46, 0, WHITE);
      } else {
        attack = 0;
      }
    } else if (attack == 2) {
      int num = sprite_counter % 75 / 15;
      if (num < 4) {
        DrawTextureProFast(resource->attack2[num], pos.x() + DRAW_X - 26,
                           pos.y() + DRAW_Y - 46, 0, WHITE);
      } else {
        attack = 0;
      }
    } else if (attack == 3) {
      int num = sprite_counter % 72 / 12;
      if (num < 5) {
        DrawTextureProFast(resource->attack3[num], pos.x() + DRAW_X - 17,
                           pos.y() + DRAW_Y - 46, 0, WHITE);
      } else {
        attack = 0;
      }
    } else {
      if (moving) {
        DrawTextureProFast(resource->walk[sprite_counter % 105 / 15],
                           pos.x() + DRAW_X - 30, pos.y() + DRAW_Y-45, 0, WHITE);
      } else {
        DrawTextureProFast(resource->idle[sprite_counter % 84 / 12], pos.x() + DRAW_X-29,
                           pos.y() + DRAW_Y-45, 0, WHITE);
      }
    }

    if (health_bar.delay > 0) {
      health_bar.draw(pos.x() + DRAW_X, pos.y() + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }

  void update() final { monster_update(); }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_SKELETONSPEAR_H_
