#ifndef MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_

namespace Server {
inline int player_num;
inline bool connected = false;
static void init() {
  NBN_UDP_Register();

  NBN_GameServer_Init(MG2_NET_IDENTIFIER, MG2_PORT, false);
  NBN_GameServer_RegisterMessage(UDP_PLAYER_POS, (NBN_MessageBuilder)MPE_PlayerPos_Create,
                                 (NBN_MessageDestructor)MPE_PlayerPos_Destroy,
                                 (NBN_MessageSerializer)MPE_PlayerPos_Serialize);
  if (NBN_GameServer_Start() < 0) {
    Log(LOG_ERROR, "Failed to start the server");
  }
  MP_TYPE = MultiplayerType::SERVER;
}

inline static void send_packets() noexcept {
  NBN_GameServer_SendPackets();
}
inline static int receive_packet() noexcept {
  NBN_MessageInfo msg_info = NBN_GameServer_GetMessageInfo();
  auto type = msg_info.type;
  switch (type) {
    case UDP_PLAYER_POS: {
      auto data = (MPE_PlayerPos*)msg_info.data;
      if (!connected) {
        MPE_PlayerPos_Destroy(data);
        return 0;
      }
      OTHER_PLAYERS[data->player_num - 1]->pos.x_ = data->x;
      OTHER_PLAYERS[data->player_num - 1]->pos.y_ = data->y;
      MPE_PlayerPos_Destroy(data);
    }
    case UDP_PLAYER_ABILITY: {
    }
  }

  return 0;
}
inline static void send_handshake() noexcept {
  //NBN_GameServer_SendReliableMessageTo()
}
static void poll_events() noexcept {
  NBN_GameServer_AddTime(MG2_TICK_TIME);
  while ((MP_EVENT_CODE = NBN_GameServer_Poll()) != NBN_NO_EVENT) {
    if (MP_EVENT_CODE < 0) {
      Log(LOG_ERROR, "Something went wrong");
    }

    switch (MP_EVENT_CODE) {
      case NBN_NEW_CONNECTION:
        if (nbn_game_server.clients->count == 3) {
          NBN_GameServer_RejectIncomingConnectionWithCode(MG2_SERVER_BUSY);
        } else {
          auto client = NBN_GameServer_GetIncomingConnection();
          OTHER_PLAYERS[0] = new NetPlayer({150, 150}, "hey", {25, 25});
          connected = true;
          NBN_GameServer_AcceptIncomingConnection();
        }
        break;

      case NBN_CLIENT_DISCONNECTED:
        //assert(NBN_GameServer_GetDisconnectedClient()->id == client->id);
        if (nbn_game_server.clients->count == 0) {
          MP_TYPE = MultiplayerType::OFFLINE;
        }
        break;

      case NBN_CLIENT_MESSAGE_RECEIVED:
        if (receive_packet() < 0) {
          Log(LOG_ERROR, "Failed to echo received message");
        }
        break;
    }
  }
}

}  // namespace Server

#endif  //MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
