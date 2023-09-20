#ifndef MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
#define MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_

struct UDP_PlayerPos {
  uint16_t x;
  uint16_t y;
};

struct UDP_PlayerPosNamed {
  uint64_t steam_id;
  uint16_t x;
  uint16_t y;
};

struct UDP_PlayerPositionBroadcast {
  uint8_t client_count;
  UDP_PlayerPosNamed clients_pos[MG2_MAX_PLAYERS];
};

struct UDP_Projectile {
  uint8_t p_type;
  int16_t x;
  int16_t y;
  float pov;
  float move_x;
  float move_y;
  float damage;
};

struct UDP_MonsterSpawn {
  uint16_t monster_id;
  MonsterType type;
  uint8_t level;
  uint16_t x;
  uint16_t y;
};

struct UDP_PlayerDamage {
  uint64_t steam_id;
  uint16_t new_health;
};

struct UDP_MonsterUpdate {
  uint16_t monster_id;
  uint16_t x;
  uint16_t y;
  uint16_t new_health;
  int8_t action_state;
};
#endif  //MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
