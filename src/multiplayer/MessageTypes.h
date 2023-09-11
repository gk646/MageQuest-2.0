#ifndef MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
#define MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_


struct MPE_PlayerPos {
  int player_num;
  int x;
  int y;
};

MPE_PlayerPos* MPE_PlayerPos_Create() {
  return (MPE_PlayerPos*)malloc(sizeof(MPE_PlayerPos));
}
void MPE_PlayerPos_Destroy(MPE_PlayerPos* msg) {
  free(msg);
  msg = nullptr;
}
int MPE_PlayerPos_Serialize(MPE_PlayerPos* msg, NBN_Stream* stream) {
  NBN_SerializeInt(stream, msg->player_num, 1, 3);
  NBN_SerializeInt(stream, msg->x, 0, 24000);
  NBN_SerializeInt(stream, msg->y, 1, 24000);
  return 0;
}
#endif  //MAGEQUEST_SRC_MULTIPLAYER_MESSAGETYPES_H_
