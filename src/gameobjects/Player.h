#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

struct Player final : public Entity {
  std::string name;
  MonsterResource* resource = &textures::PLAYER_RESOURCE;
  int actionState = 0;
  bool flip = false;
  bool moving = false;
  uint8_t uncoverRadius = 8;
  explicit Player(const Point& pos) : Entity(pos, {28, 50}, ShapeType::RECT) {
    PLAYER_TILE = &tile_pos;
  }
  Player(const Player& other) noexcept : Entity(other) {}
  Player& operator=(const Player& other) {
    if (this == &other) {
      return *this;
    }
    Entity::operator=(other);
    return *this;
  }
  static void Hit(Projectile& p) noexcept {
    //TODO dodge chance
    if (!p.from_player && p.IsActive()) {
      PLAYER_EFFECTS.AddEffects(p.statusEffects);
      PLAYER_STATS.TakeDamage(p.damageStats);
      p.isDead = p.hitType == HitType::ONE_HIT;
    }
  }
  void Update() final {
    if (isDead) {
      GAME_STATE = GameState::GameOver;
      return;
    } else if (PLAYER_STATS.stunned) {
      return;
    }

    float speed = PLAYER_STATS.GetSpeed();

    bool verticalMove = (IsKeyDown(KEY_W) || IsKeyDown(KEY_S));
    bool horizontalMove = (IsKeyDown(KEY_A) || IsKeyDown(KEY_D));

    if (verticalMove && horizontalMove) {
      speed /= SQRT_2;
    }

    moving = verticalMove || horizontalMove;
    if (IsKeyDown(KEY_W) && !tile_collision_up(speed)) {
      pos.y_ -= speed;
      GAME_STATISTICS.WalkPixels(speed);
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(speed)) {
      pos.y_ += speed;
      GAME_STATISTICS.WalkPixels(speed);
    }
    if (IsKeyDown(KEY_A) && !tile_collision_left(speed)) {
      pos.x_ -= speed;
      GAME_STATISTICS.WalkPixels(speed);
      flip = true;
    }
    if (IsKeyDown(KEY_D) && !tile_collision_right(speed)) {
      pos.x_ += speed;
      GAME_STATISTICS.WalkPixels(speed);
      flip = false;
    }

    tile_pos.x = static_cast<int>(pos.x_ + size.x / 2) / TILE_SIZE;
    tile_pos.y = static_cast<int>(pos.y_ + size.y / 2) / TILE_SIZE;

    UncoverMapCover();
    Multiplayer::UDP_SEND_POSITION(static_cast<int16_t>(pos.x_),
                                   static_cast<int16_t>(pos.y_));
    spriteCounter++;
  }
  void Draw() final {
    if (moving) {
      DrawTextureProFastEx(resource->walk[spriteCounter % 56 / 7],
                           std::floor(pos.x_ + DRAW_X - 25.0F), std::floor(pos.y_ + DRAW_Y - 45),
                           -23, 0, flip, WHITE);
      actionState = 0;
    } else if (actionState == 1) {
      draw_attack1();
    } else if (actionState == 2) {
      draw_attack2();
    } else if (actionState == 3) {
      draw_attack3();
    } else if (actionState == -100) {
      draw_death();
    }
    if (!moving && actionState == 0) {
      DrawTextureProFastEx(resource->idle[spriteCounter % 80 / 10],
                           pos.x_ + DRAW_X - 32.0F, pos.y_ + DRAW_Y - 46, -7, 0, flip,
                           WHITE);
    }
    DRAW_HITBOXES();
  }
  inline void draw_death() noexcept {
    int num = spriteCounter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = spriteCounter % 48 / 6;
    if (num < 7) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = spriteCounter % 50 / 5;
    if (num < 9) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -20, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = spriteCounter % 85 / 5;
    if (num < 16) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -15, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }

 private:
  inline void UncoverMapCover() noexcept {
    int sx = tile_pos.x;
    int sy = tile_pos.y;
    int radiusSqrt = uncoverRadius * uncoverRadius;
    int xMin = std::max(sx - uncoverRadius, 0);
    int xMax = std::min(sx + uncoverRadius, CURRENT_MAP_SIZE);
    int yMin = std::max(sy - uncoverRadius, 0);
    int yMax = std::min(sy + uncoverRadius, CURRENT_MAP_SIZE);

    for (int x = xMin; x < xMax; ++x) {
      for (int y = yMin; y < yMax; ++y) {
        if (RANGE_01(RNG_ENGINE) > 0.92F &&
            (x - sx) * (x - sx) + (y - sy) * (y - sy) <= radiusSqrt) {
          CURRENT_MAP_COVER[x][y] = false;
        }
      }
    }
  }
};
inline static Player PLAYER({150, 150});
#include "../ui/player/HotBar.h"
#include "WorldObject.h"
void EntityStats::RemoveEffects() noexcept {
  PLAYER_EFFECTS.RemoveEffects();
}
void EntityStats::ApplyEffects() noexcept {
  PLAYER_EFFECTS.ApplyEffects();
}

bool SpawnTrigger::IsClose() const noexcept {
  return PLAYER.pos.dist(pos.x + size.x / 2, pos.y + size.y / 2) < UPDATE_DISTANCE * 48;
}
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
