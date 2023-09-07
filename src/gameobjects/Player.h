#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

struct Player : public Entity {
  std::string name;
  MonsterResource resource;
  explicit Player(const Point& pos, const Point& size = {25, 25})
      : Entity(pos, size, ShapeType::RECT), name("New Player") {}
  Player(const Player& other) noexcept : Entity(other), name(other.name) {
  }
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
    DrawRectanglePro(CAMERA_X - size.x() / 2, CAMERA_Y - size.y() / 2, size.x(),
                     size.y(), {0, 0}, pov, BLUE);
  }
  static void hit(Projectile& p) noexcept {
    if (!p.from_player) {
      PLAYER_EFFECTS.add_effects(p.status_effects);
      PLAYER_STATS.take_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
  void update() final {}
  void movement() {
    float speed = PLAYER_STATS.get_speed();
    if (IsKeyDown(KEY_W) && !tile_collision_up(speed)) {
      pos.y() -= speed;
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(speed)) {
      pos.y() += speed;
    }
    if (IsKeyDown(KEY_A) && !tile_collision_left(speed)) {
      pos.x() -= speed;
    }
    if (IsKeyDown(KEY_D) && !tile_collision_right(speed)) {
      pos.x() += speed;
    }
    PLAYER_TILE_X = (pos.x() + size.x() / 2) / TILE_SIZE;
    PLAYER_TILE_Y = (pos.y() + size.y() / 2) / TILE_SIZE;
  }
};
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
