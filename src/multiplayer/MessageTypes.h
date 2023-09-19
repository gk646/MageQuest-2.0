#ifndef MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
#define MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_

struct UDP_PlayerPos {
  uint16_t x;
  uint16_t y;
};

struct UDP_PlayerPos_Update {
  uint64_t steam_id;
  uint16_t x;
  uint16_t y;
};

struct UDP_PlayerPositionBroadcast {
  uint8_t client_count;
  UDP_PlayerPos_Update clients_pos[MG2_MAX_PLAYERS];
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

#endif  //MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
