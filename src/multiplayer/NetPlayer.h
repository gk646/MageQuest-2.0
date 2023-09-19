#ifndef MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_

struct NetPlayer final : public Entity {
  EntityStats stats;
  MonsterResource* resource = &textures::PLAYER_RESOURCE;
  std::string name;
  StatusEffectHandler status_effects{stats};
  SteamNetworkingIdentity identity = SteamNetworkingIdentity();
  Zone zone = Zone::Woodland_Edge;
  int sprite_counter = 0;
  bool moving = false;
  bool prev_moving = false;
  int action_state = 0;
  bool flip = false;
  explicit NetPlayer(const Point& pos, Zone zone, CSteamID steam_id, int client_id,
                     const Point& size = {28, 50})
      : Entity(pos, size, ShapeType::RECT),
        zone(zone),
        name(SteamFriends()->GetFriendPersonaName(steam_id)) {
    identity.SetSteamID(steam_id);
  }
  void draw() final {
    if (zone != CURRENT_ZONE) {
      return;
    }
    if (moving) {
      DrawTextureProFastEx(resource->walk[sprite_counter % 64 / 8], pos.x_ + DRAW_X - 25,
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
    DrawTextPro(
        ANT_PARTY, name.c_str(),
        {pos.x_ + DRAW_X - MeasureTextEx(MINECRAFT_REGULAR, name.c_str(), 15, 0.5).x / 2,
         pos.y_ + DRAW_Y},
        {0, 0}, 0, 15, 0.5, WHITE);
  }
  void update() final {
    sprite_counter++;
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }
  inline void update_state(uint16_t x, uint16_t y) noexcept {
    if(pos.x_ == x && pos.y_ == y){
      if(prev_moving){
        prev_moving = false;
      }else{
        moving = false;
      }
    }else {
      moving = true;
      prev_moving = moving;
    }

    if (x < pos.x_) {
      flip = true;
    } else if (x > pos.x_) {
      flip = false;
    }

    pos.x_ = x;
    pos.y_ = y;

  }
  void hit(Projectile& p) noexcept {
    if (!p.from_player) {
      status_effects.add_effects(p.status_effects);
      stats.take_damage(p.damage_stats);
      p.dead = p.projectile_type == HitType::ONE_HIT;
    }
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
#endif  //MAGEQUEST_SRC_MULTIPLAYER_NETPLAYER_H_
