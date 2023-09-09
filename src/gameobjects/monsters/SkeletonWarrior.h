#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_

struct SkeletonWarrior final : public Monster {
  static constexpr int base_health = 10;
  static constexpr float base_speed = 2;
  SkeletonWarrior(const Point& pos, int level) noexcept
      : Monster(pos, EntityStats{base_health, level, base_speed},
                &textures::SKELETON_WARRIOR, {35, 47}) {}

  void draw() final {
#ifdef DRAW_HITBOXES
    DrawRectangle(tile_pos.x * 48 + DRAW_X, tile_pos.y * 48 + DRAW_Y, 48, 48, GREEN);
#endif
    if (pos.x() + size.x() / 2 > PLAYER.pos.x() + PLAYER.size.x() / 2) {
      draw_mirrored();
    } else {
      if (attack == -100) {
        int num = sprite_counter % 175 / 35;
        if (num < 4) {
          DrawTextureProFast(resource->death[num], pos.x() + DRAW_X - 25,
                             pos.y() + DRAW_Y - 46, 0, WHITE);
        } else {
          dead = true;
        }
      } else if (attack == 1) {
        int num = sprite_counter % 96 / 16;
        if (num < 5) {
          DrawTextureProFast(resource->attack1[num], pos.x() + DRAW_X - 23,
                             pos.y() + DRAW_Y - 13, 0, WHITE);
        } else {
          attack = 0;
        }
      } else if (attack == 2) {
        int num = sprite_counter % 112 / 16;
        if (num < 6) {
          DrawTextureProFast(resource->attack2[num], pos.x() + DRAW_X - 32,
                             pos.y() + DRAW_Y - 20, 0, WHITE);
        } else {
          attack = 0;
        }
      } else if (attack == 3) {
        int num = sprite_counter % 60 / 12;
        if (num < 4) {
          DrawTextureProFast(resource->attack3[num], pos.x() + DRAW_X - 23,
                             pos.y() + DRAW_Y - 13, 0, WHITE);
        } else {
          attack = 0;
        }
      } else {
        if (moving) {
          DrawTextureProFast(resource->walk[sprite_counter % 140 / 20],
                             pos.x() + DRAW_X - 35, pos.y() + DRAW_Y - 5, 0, WHITE);
        } else {
          DrawTextureProFast(resource->idle[sprite_counter % 112 / 16], pos.x() + DRAW_X,
                             pos.y() + DRAW_Y, 0, WHITE);
        }
      }
    }
    if (health_bar.delay > 0) {
      health_bar.draw(pos.x() + DRAW_X, pos.y() + DRAW_Y, stats);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }

  void draw_mirrored() noexcept {
    if (attack == -100) {
      int num = sprite_counter % 175 / 35;
      if (num < 4) {
        auto texture = resource->death[num];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x() + DRAW_X, pos.y() + DRAW_Y - 46,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        dead = true;
      }
    } else if (attack == 1) {
      int num = sprite_counter % 96 / 16;
      if (num < 5) {
        auto texture = resource->attack1[num];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x() + DRAW_X -40, pos.y() + DRAW_Y - 13,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        attack = 0;
      }
    } else if (attack == 2) {
      int num = sprite_counter % 112 / 16;
      if (num < 6) {
        auto texture = resource->attack2[num];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x() + DRAW_X -37, pos.y() + DRAW_Y - 20,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        attack = 0;
      }
    } else if (attack == 3) {
      int num = sprite_counter % 60 / 12;
      if (num < 4) {
        auto texture = resource->attack3[num];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x() + DRAW_X-40, pos.y() + DRAW_Y - 13,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        attack = 0;
      }
    } else {
      if (moving) {
        auto texture = resource->walk[sprite_counter % 140 / 20];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x() + DRAW_X - 40, pos.y() + DRAW_Y - 5,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      } else {
        auto texture = resource->idle[sprite_counter % 112 / 16];
        RectangleR srcRec = {0, 0, static_cast<float>(-texture.width),
                             static_cast<float>(texture.height)};
        RectangleR destRec = {pos.x() + DRAW_X, pos.y() + DRAW_Y,
                              static_cast<float>(texture.width),
                              static_cast<float>(texture.height)};
        DrawTexturePro(texture, srcRec, destRec, {0, 0}, 0, WHITE);
      }
    }
  }

  void update() final {
    monster_update();
    if (move_to_player() && attack == 0) {
      int num = RANGE_100(RNG_ENGINE);
      sprite_counter = 0;
      if (num < 33) {
        attack = 1;
      } else if (num < 66) {
        attack = 2;
      } else {
        attack = 3;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_MONSTERS_SKELETONWARRIOR_H_
