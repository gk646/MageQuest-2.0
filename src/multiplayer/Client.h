#ifndef MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
#define MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_

namespace Client {
inline int PLAYER_NUM;
inline bool connected = false;
inline static void init(const char* ip) noexcept {
  NBN_UDP_Register();
  NBN_GameClient_Init(MG2_NET_IDENTIFIER, ip, MG2_PORT, false, nullptr);
  NBN_GameClient_RegisterMessage(UDP_PLAYER_POS, (NBN_MessageBuilder)MPE_PlayerPos_Create,
                                 (NBN_MessageDestructor)MPE_PlayerPos_Destroy,
                                 (NBN_MessageSerializer)MPE_PlayerPos_Serialize);

  NBN_GameClient_RegisterMessage(UDP_PLAYER_ABILITY,
                                 (NBN_MessageBuilder)MPE_PlayerPos_Create,
                                 (NBN_MessageDestructor)MPE_PlayerPos_Destroy,
                                 (NBN_MessageSerializer)MPE_PlayerPos_Serialize);
  if (NBN_GameClient_Start() < 0) {
    Log(LOG_ERROR, "Failed to start client");
  }
  MP_TYPE = MultiplayerType::CLIENT;
}
inline static void send_packets() noexcept {
  if (NBN_GameClient_SendPackets() < 0) {
    Log(LOG_ERROR, "Failed to send packets. Exit");
  }
}
inline static int receive_packet() noexcept {
  if(!connected ) return 0;
  NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();
  auto type = msg_info.type;
  switch (type) {
    case UDP_PLAYER_POS: {
      auto data = (MPE_PlayerPos*)msg_info.data;
      OTHER_PLAYERS[data->player_num - 1]->pos.x_ = data->x;
      OTHER_PLAYERS[data->player_num - 1]->pos.y_ = data->y;
      MPE_PlayerPos_Destroy(data);
    }
    case UDP_PLAYER_ABILITY: {
    }
  }
  return 0;
}
inline static void poll_events() noexcept {
  NBN_GameClient_AddTime(MG2_TICK_TIME);
  while ((MP_EVENT_CODE = NBN_GameClient_Poll()) != NBN_NO_EVENT) {
    if (MP_EVENT_CODE < 0) {
      Log(LOG_ERROR, "An error occured while polling client events. Exit");
      break;
    }

    switch (MP_EVENT_CODE) {
      case NBN_CONNECTED:
        connected = true;
        OTHER_PLAYERS[0] = new NetPlayer({150, 150}, "hey", {25, 25});
        NBN_GameClient_SendReliableMessage(UDP_PLAYER_POS,
                                           new MPE_PlayerPos(1, 150, 150));
        break;

      case NBN_DISCONNECTED:
        MP_TYPE = MultiplayerType::OFFLINE;
        break;

      case NBN_MESSAGE_RECEIVED:
        if (receive_packet() < 0) {
          Log(ERROR, "Error receiving packet");
        }
        break;
    }
  }
}

}  // namespace Client
#endif  //MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
