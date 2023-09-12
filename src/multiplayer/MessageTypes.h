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
  NBN_SerializeInt(stream, msg->x, 0, 24000);
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

UDP_PlayerPos_Update* UDP_PlayerPos_Update_Create() {
  return (UDP_PlayerPos_Update*)malloc(sizeof(UDP_PlayerPos_Update));
}
void UDP_PlayerPos_Update_Destroy(UDP_PlayerPos_Update* msg) {
  free(msg);
}
int UDP_PlayerPos_Update_Serialize(UDP_PlayerPos_Update* msg, NBN_Stream* stream) {
  NBN_SerializeInt(stream, msg->client_id, 0, 3);
  NBN_SerializeInt(stream, msg->x, 0, 24000);
  NBN_SerializeInt(stream, msg->y, 0, 24000);
  return 0;
}

struct UDP_PositionState {
  int client_count;
  UDP_PlayerPos_Update clients_pos[MG2_MAX_PLAYERS];
};

UDP_PositionState* UDP_PositionState_Create() {
  return (UDP_PositionState*)malloc(sizeof(UDP_PositionState));
}
void UDP_PlayerPos_BroadCast_Destroy(UDP_PositionState* msg) {
  free(msg);
}
int UDP_PlayerPos_BroadCast_Serialize(UDP_PositionState* msg, NBN_Stream* stream) {
  NBN_SerializeInt(stream, msg->client_count, 0, 3);
  for (uint_fast32_t i = 0; i < msg->client_count; i++) {
    NBN_SerializeInt(stream, msg->clients_pos[i].client_id, 0, 3);
    NBN_SerializeInt(stream, msg->clients_pos[i].x, 0, 24000);
    NBN_SerializeInt(stream, msg->clients_pos[i].y, 0, 24000);
  }
  return 0;
}

#endif  //MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
