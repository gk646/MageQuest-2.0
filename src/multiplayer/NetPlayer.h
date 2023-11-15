#ifndef MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_

struct NetPlayer final : public Entity {
  EntityStats stats;
  MonsterResource* resource = &textures::PLAYER_RESOURCE;
  std::string name;
  StatusEffectHandler status_effects{stats, this};
  SteamNetworkingIdentity identity = SteamNetworkingIdentity();
  Zone zone = Zone::Tutorial;
  int sprite_counter = 0;
  bool moving = false;
  bool prev_moving = false;
  bool flip = false;
  explicit NetPlayer(const Point& pos, Zone zone, CSteamID steam_id,
                     const PointT<int16_t>& size = {28, 50})
      : Entity(pos, size, ShapeType::RECT),
        zone(zone),
        name(SteamFriends()->GetFriendPersonaName(steam_id)) {
    identity.SetSteamID(steam_id);
  }
  void Draw() final {
    if (zone != CURRENT_ZONE) {
      return;
    }
    draw_direction_indicator();
    if (moving) {
      DrawTextureProFastEx(resource->walk[sprite_counter % 64 / 8], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -23, 0, flip, WHITE);
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
      DrawTextureProFastEx(resource->idle[sprite_counter % 80 / 10], pos.x_ + DRAW_X - 30,
                           pos.y_ + DRAW_Y - 45, -7, 0, flip, WHITE);
    }
    DRAW_HITBOXES();
    DrawTextPro(
        VARNISHED, name.c_str(),
        {pos.x_ + DRAW_X - MeasureTextEx(MINECRAFT_REGULAR, name.c_str(), 14, 0.5).x / 2,
         pos.y_ + DRAW_Y - 15},
        {0, 0}, 0, 14, 0.5, WHITE);
  }
  void Update() final {
    sprite_counter++;
    tilePos.x = (pos.x_ + size.x / 2) / TILE_SIZE;
    tilePos.y = (pos.y_ + size.y / 2) / TILE_SIZE;
  }
  inline void update_state(uint16_t x, uint16_t y) noexcept {
    if (pos.x_ == x && pos.y_ == y) {
      if (prev_moving) {
        prev_moving = false;
        return;
      }
      moving = false;
    } else {
      moving = true;
      prev_moving = true;
    }

    flip = (x < pos.x_);
    pos.x_ = x;
    pos.y_ = y;
  }
  inline void Hit(Projectile& p) noexcept {
    if (!p.isFriendlyToPlayer) {
      p.HitTargetCallback();
      status_effects.AddEffects(p.statusEffects);
      stats.TakeDamage(p.damageStats, this);
    }
  }
  inline void draw_direction_indicator() noexcept {
    //TODO mouse direction indicator
  }
  inline void draw_death() noexcept {
    int num = sprite_counter % 75 / 15;
    if (num < 4) {
      DrawTextureProFastEx(resource->death[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      isDead = true;
    }
  }
  inline void draw_attack1() noexcept {
    int num = sprite_counter % 48 / 6;
    if (num < 7) {
      DrawTextureProFastEx(resource->attack1[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -22, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack2() noexcept {
    int num = sprite_counter % 50 / 5;
    if (num < 9) {
      DrawTextureProFastEx(resource->attack2[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -20, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
  inline void draw_attack3() noexcept {
    int num = sprite_counter % 85 / 5;
    if (num < 16) {
      DrawTextureProFastEx(resource->attack3[num], pos.x_ + DRAW_X - 25,
                           pos.y_ + DRAW_Y - 45, -15, 0, flip, WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_
