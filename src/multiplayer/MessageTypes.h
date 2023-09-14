#ifndef MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
#define MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_

struct UDP_PlayerPos {
  int x;
  int y;
};

UDP_PlayerPos* UDP_PlayerPos_Client_Create() {
  return (UDP_PlayerPos*)malloc(sizeof(UDP_PlayerPos));
}
void UDP_PlayerPos_Client_Destroy(UDP_PlayerPos* msg) {
  free(msg);
}
int UDP_PlayerPos_Client_Serialize(UDP_PlayerPos* msg, NBN_Stream* stream) {
  NBN_SerializeInt(stream, msg->x, 0, 24000)
  NBN_SerializeInt(stream, msg->y, 0, 24000);
  return 0;
}

struct UDP_PlayerName {
  unsigned int len;
  char* name;
};

UDP_PlayerName* UDP_PlayerName_Create() {
  return (UDP_PlayerName*)malloc(sizeof(UDP_PlayerName));
}
void UDP_PlayerName_Destroy(UDP_PlayerName* msg) {
  free(msg);
}
int UDP_PlayerName_Serialize(UDP_PlayerName* msg, NBN_Stream* stream) {
  NBN_SerializeUInt(stream, msg->len, 0, 35);
  NBN_SerializeBytes(stream, msg->name, msg->len);
  return 0;
}

struct UDP_PlayerPos_Update {
  int client_id;
  int x;
  int y;
};

struct UDP_PositionState {
  int client_count;
  UDP_PlayerPos_Update clients_pos[MG2_MAX_PLAYERS];
};
UDP_PositionState* UDP_PositionState_Create() {
  return (UDP_PositionState*)malloc(sizeof(UDP_PositionState));
}
void UDP_PositionState_Destroy(UDP_PositionState* msg) {
  free(msg);
}
int UDP_PositionState_Serialize(UDP_PositionState* msg, NBN_Stream* stream) {
  NBN_SerializeInt(stream, msg->client_count, 0, 3);
  for (uint_fast32_t i = 0; i < msg->client_count; i++) {
    NBN_SerializeInt(stream, msg->clients_pos[i].client_id, 0, 3);
    NBN_SerializeInt(stream, msg->clients_pos[i].x, 0, 24000);
    NBN_SerializeInt(stream, msg->clients_pos[i].y, 0, 24000);
  }
  return 0;
}

struct UDP_Projectile {
  int p_type;
  int x;
  int y;
  float pov;
  float move_x;
  float move_y;
  float damage;
};

UDP_Projectile* UDP_Projectile_Create() {
  return (UDP_Projectile*)malloc(sizeof(UDP_Projectile));
}
void UDP_Projectile_Destroy(UDP_Projectile* msg) {
  free(msg);
}
int UDP_Projectile_Serialize(UDP_Projectile* msg, NBN_Stream* stream) {
  NBN_SerializeInt(stream, msg->p_type, 0, 50);
  NBN_SerializeInt(stream, msg->x, -24000, 24000);
  NBN_SerializeInt(stream, msg->y, -24000, 24000);

  NBN_SerializeFloat(stream, msg->pov, -360, 360, 3);
  NBN_SerializeFloat(stream, msg->move_x, -50, 50, 3);
  NBN_SerializeFloat(stream, msg->move_y, -50, 50, 3);
  NBN_SerializeFloat(stream, msg->damage, 0, 1000000, 3);
  return 0;
}

#define UDP_PROJECTILE_UPDATE_SIG                                                     \
  NBN_RPC_BuildSignature(7, NBN_RPC_PARAM_INT, NBN_RPC_PARAM_INT, NBN_RPC_PARAM_INT,  \
                         NBN_RPC_PARAM_INT, NBN_RPC_PARAM_FLOAT, NBN_RPC_PARAM_FLOAT, \
                         NBN_RPC_PARAM_FLOAT)

#endif  //MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
