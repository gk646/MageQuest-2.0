#include <utility>

#ifndef MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_

struct NetPlayer : Entity {
  EntityStats stats;
  MonsterResource* resource = nullptr;
  std::string name;
  StatusEffectHandler status_effects{stats};
  explicit NetPlayer(const Point& pos, std::string name, const Point& size = {25, 25})
      : Entity(pos, size, ShapeType::RECT), name(std::move(name)) {
    PLAYER_TILE = &tile_pos;
  }
  void draw() final {
    DrawRectanglePro(pos.x_ + DRAW_X, pos.y_ + DRAW_Y, size.x_, size.y_, {0, 0}, pov,
                     PURPLE);
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void hit(Projectile& p) noexcept {
    if (!p.from_player) {
      status_effects.add_effects(p.status_effects);
      stats.take_damage(p.damage_stats);
      p.dead = p.projectile_type == ProjectileType::ONE_HIT;
    }
  }
  void update() final {
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }
};
#endif  //MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_
