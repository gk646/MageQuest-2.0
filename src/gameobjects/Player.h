#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

struct Player final : public Entity {
  std::string name;
  MonsterResource* resource = &textures::PLAYER_RESOURCE;
  int action_state = 0;
  bool flip = false;
  bool moving = false;
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
  static void hit(Projectile& p) noexcept {
    if (!p.from_player && p.active()) {
      PLAYER_EFFECTS.AddEffects(p.status_effects);
      PLAYER_STATS.take_damage(p.damage_stats);
      p.dead = p.projectile_type == HitType::ONE_HIT;
    }
  }
  void update() final {
    if (dead) {
      GAME_STATE = GameState::GameOver;
    }
    float speed = PLAYER_STATS.get_speed();
    if(PLAYER_STATS.stunned) {
     return;
    }

    moving = false;
    if (IsKeyDown(KEY_W) && !tile_collision_up(speed)) {
      pos.y_ -= speed;
      moving = true;
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(speed)) {
      pos.y_ += speed;
      moving = true;
    }
    if (IsKeyDown(KEY_A) && !tile_collision_left(speed)) {
      pos.x_ -= speed;
      flip = true;
      moving = true;
    }
    if (IsKeyDown(KEY_D) && !tile_collision_right(speed)) {
      pos.x_ += speed;
      flip = false;
      moving = true;
    }
    if(moving){
      GAME_STATISTICS.WalkPixels(speed);
    }
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;

    Multiplayer::UDP_SEND_POSITION(static_cast<int16_t>(pos.x_), static_cast<int16_t>(pos.y_));
    sprite_counter++;
  }
  void draw() final {
    if (moving) {
      DrawTextureProFastEx(resource->walk[sprite_counter % 56 / 7], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -23, 0, flip, WHITE);
      action_state = 0;
    } else if (action_state == 1) {
      draw_attack1();
    } else if (action_state == 2) {
      draw_attack2();
    } else if (action_state == 3) {
      draw_attack3();
    } else if (action_state == -100) {
      draw_death();
    }
    if (!moving && action_state == 0) {
      DrawTextureProFastEx(resource->idle[sprite_counter % 80 / 10], pos.x_ + DRAW_X - 30,
                           pos.y_ + DRAW_Y - 45, -7, 0, flip, WHITE);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  inline void draw_death() noexcept {
    int num = sprite_counter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      dead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = sprite_counter % 48 / 6;
    if (num < 7) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      action_state = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = sprite_counter % 50 / 5;
    if (num < 9) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -20, 0, flip, WHITE);
    } else {
      action_state = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = sprite_counter % 85 / 5;
    if (num < 16) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -15, 0, flip, WHITE);
    } else {
      action_state = 0;
    }
  }
};
inline static Player PLAYER({150, 150});
#include "elements/ThreatManager.h"
#include "../ui/player/HotBar.h"
inline static HotBar PLAYER_HOTBAR{};
void EntityStats::RemoveEffects() noexcept {
  PLAYER_EFFECTS.RemoveEffects();
}
void EntityStats::ApplyEffects() noexcept {
  PLAYER_EFFECTS.ApplyEffects();
}
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
