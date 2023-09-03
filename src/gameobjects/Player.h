#ifndef MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
#define MAGE_QUEST_SRC_ENTITIES_PLAYER_H_

struct Player : public Entity {
  std::string name;
  float pov;
  StatusEffectHandler status_effects{PLAYER_STATS};
  explicit Player(const Point& pos, const Point& size = {25, 25})
      : Entity(pos, size, ShapeType::RECT), pov(0), name("New Player") {}
  Player(const Player& other)
      : Entity(other), name(other.name), pov(other.pov) {}
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
    DrawRectanglePro(CAMERA_X - size.x() / 2, CAMERA_Y - size.y() / 2, size.x(), size.y(), {0, 0},
                     pov, BLUE);
  }
  void hit(Projectile& p) {
    if (!p.from_player) {
      status_effects.add_effects(p.status_effects);
      PLAYER_STATS.general.take_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
  void abilities() {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      status_effects.add_effects({new Slow(50, 120)});
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      PROJECTILES.push_back(FireBall(pos,  true));
    }
  }
  void update() final {
    movement();
    PLAYER_HOTBAR.update();
    abilities();
    status_effects.update();
    PLAYER_TILE_X = (pos.x() + size.x() / 2) / TILE_SIZE;
    PLAYER_TILE_Y = (pos.y() + size.y() / 2) / TILE_SIZE;
  }
  void movement() {
    if (IsKeyDown(KEY_W) && !tile_collision_up(PLAYER_STATS.general.speed)) {
      pos.y() -= PLAYER_STATS.general.speed;
    }
    if (IsKeyDown(KEY_S) && !tile_collision_down(PLAYER_STATS.general.speed)) {
      pos.y() += PLAYER_STATS.general.speed;
    }
    if (IsKeyDown(KEY_A) && !tile_collision_left(PLAYER_STATS.general.speed)) {
      pos.x() -= PLAYER_STATS.general.speed;
    }
    if (IsKeyDown(KEY_D) && !tile_collision_right(PLAYER_STATS.general.speed)) {
      pos.x() += PLAYER_STATS.general.speed;
    }

    PLAYER_X = pos.x();
    PLAYER_Y = pos.y();
  }
};
#endif  //MAGE_QUEST_SRC_ENTITIES_PLAYER_H_
