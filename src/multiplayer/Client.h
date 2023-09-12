#ifndef MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
#define MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_

namespace Client {
inline bool connected = false;
inline int client_id = 0;
inline static void init(const char* ip) noexcept {
  connected = false;
  client_id = -1;
  auto send_data = new uint8_t[3 + PLAYER_NAME.size() + 1];
  send_data[0] = (uint8_t)CURRENT_ZONE;
  send_data[0] = (uint8_t)PLAYER_X;
  send_data[0] = (uint8_t)PLAYER_Y;
  transcribe_string_into_uint8(3, PLAYER_NAME, send_data);
  NBN_GameClient_Init(MG2_NET_IDENTIFIER, ip, MG2_PORT, false, send_data);

  NBN_GameClient_RegisterMessage(UDP_PLAYER_POS_CLIENT,
                                 (NBN_MessageBuilder)UDP_PlayerPos_Client_Create,
                                 (NBN_MessageDestructor)UDP_PlayerPos_Client_Destroy,
                                 (NBN_MessageSerializer)UDP_PlayerPos_Client_Serialize);
  NBN_GameClient_RegisterMessage(
      UDP_PLAYER_STATE, (NBN_MessageBuilder)UDP_PositionState_Create,
      (NBN_MessageDestructor)UDP_PlayerPos_BroadCast_Destroy,
      (NBN_MessageSerializer)UDP_PlayerPos_BroadCast_Serialize);
  NBN_GameClient_RegisterMessage(UDP_PLAYER_NAME,
                                 (NBN_MessageBuilder)UDP_PlayerName_Create,
                                 (NBN_MessageDestructor)UDP_PlayerName_Destroy,
                                 (NBN_MessageSerializer)UDP_PlayerName_Serialize);
  if (NBN_GameClient_Start() < 0) {
    Log(NET_LOG_ERROR, "Failed to start client");
  }
  delete[] send_data;
  MP_TYPE = MultiplayerType::CLIENT;
}
inline static void send_packets() noexcept {
  if (NBN_GameClient_SendPackets() < 0) {
    Log(NET_LOG_ERROR, "Failed to send packets. Exit");
  }
}
inline static void HandleConnection() noexcept {
  NBN_Stream* rs = NBN_GameClient_GetAcceptDataReadStream();

  int x = 0, y = 0, id = 0;
  NBN_SerializeInt(rs, x, 0, 24000);
  NBN_SerializeInt(rs, y, 0, 24000);
  NBN_SerializeInt(rs, id, 0, 4);
  client_id = id;
  std::cout << id << std::endl;
  OTHER_PLAYERS[0] = new NetPlayer({(float)x, (float)y}, "hey ", Zone(0), nullptr, 1);
  connected = true;
}
inline static void HandleDisconnection() {
  int code = NBN_GameClient_GetServerCloseCode();
  std::cout<< code << std::endl;
  switch (code) {
    case MG2_HOST_CLOSED_GAMED:
      Log(NET_LOG_INFO, "Host closed game");
  }
  MP_TYPE = MultiplayerType::OFFLINE;
  for (auto& net_player : OTHER_PLAYERS) {
    delete net_player;
    net_player = nullptr;
  }
  NBN_GameClient_Disconnect();
  NBN_GameClient_Stop();
}
inline static int receive_packet() noexcept {
  if (!connected)
    return 0;
  NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();
  auto type = msg_info.type;
  switch (type) {
    case UDP_PLAYER_STATE: {
      auto msg = (UDP_PositionState*)msg_info.data;
      for (uint_fast32_t j = 0; j < msg->client_count; j++) {
        if (msg->clients_pos[j].client_id == client_id)
          continue;
        if (OTHER_PLAYERS[j]) {
          OTHER_PLAYERS[j]->pos.x_ = msg->clients_pos[j].x;
          OTHER_PLAYERS[j]->pos.y_ = msg->clients_pos[j].y;
        } else {
          OTHER_PLAYERS[j] = new NetPlayer({(float)msg->clients_pos[j].x, (float)msg->clients_pos[j].y},
                                           "", CURRENT_ZONE, nullptr, 50);
        }
      }
    }
  }
  return 0;
}
inline static void poll_events() noexcept {
  NBN_GameClient_AddTime(MG2_TICK_TIME);
  while ((MP_EVENT_CODE = NBN_GameClient_Poll()) != NBN_NO_EVENT) {
    if (MP_EVENT_CODE < 0) {
      Log(NET_LOG_ERROR, "An error occurred while polling client events. Exit");
      break;
    }

    switch (MP_EVENT_CODE) {
      case NBN_CONNECTED:
        HandleConnection();
        break;

      case NBN_DISCONNECTED:
        HandleDisconnection();
        break;

      case NBN_MESSAGE_RECEIVED:
        if (receive_packet() < 0) {
          Log(NET_LOG_ERROR, "Error receiving packet");
        }
        break;
    }
  }
}

}  // namespace Client
#endif  //MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
