#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

struct Player : public Entity {
  MonsterResource* resource = nullptr;
  std::string name;
  explicit Player(const Point& pos, const Point& size = {25, 25})
      : Entity(pos, size, ShapeType::RECT), name("New Player") {
    PLAYER_TILE = &tile_pos;
  }
  Player(const Player& other) noexcept : Entity(other), name(other.name) {}
  Player& operator=(const Player& other) {
    if (this == &other) {
      return *this;
    }
    Entity::operator=(other);

    name = other.name;
    pov = other.pov;
    return *this;
  }
  void draw() final {
    DrawRectanglePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_, {0, 0}, pov,
                     BLUE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  static void hit(Projectile& p) noexcept {
    if (!p.from_player) {
      PLAYER_EFFECTS.add_effects(p.status_effects);
      PLAYER_STATS.take_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
  void update() final {
    float speed = PLAYER_STATS.get_speed();

    if (IsKeyDown(KEY_W) && !tile_collision_up(speed)) {
      pos.y_ -= speed;
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(speed)) {
      pos.y_ += speed;
    }
    if (IsKeyDown(KEY_A) && !tile_collision_left(speed)) {
      pos.x_ -= speed;
    }
    if (IsKeyDown(KEY_D) && !tile_collision_right(speed)) {
      pos.x_ += speed;
    }
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
    Multiplayer::send_event(UDP_PLAYER_POS, MP_TYPE == MultiplayerType::OFFLINE
                                                ? nullptr
                                                : new MPE_PlayerPos(1, pos.x_, pos.y_));
  }
};
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
