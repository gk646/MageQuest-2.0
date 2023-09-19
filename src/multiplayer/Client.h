#ifndef MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
#define MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_

namespace Client {
static void SyncPlayers() noexcept {
  int numMembers = SteamMatchmaking()->GetNumLobbyMembers(LOBBY_ID);
  for (int i = 0; i < numMembers; i++) {
    CSteamID memberId = SteamMatchmaking()->GetLobbyMemberByIndex(LOBBY_ID, i);
    if (memberId != SteamUser()->GetSteamID()) {
      const char* inGameName =
          SteamMatchmaking()->GetLobbyMemberData(LOBBY_ID, memberId, "InGameName");
      if (inGameName && *inGameName) {
        Multiplayer::add(memberId);
        Multiplayer::get(memberId)->name = inGameName;
      }
    }
  }
}
//Takes ownership of the passed msg pointer and deletes it
inline static void SendMsgToHost(uint8_t channel, const void* msg,
                                 uint32_t length) noexcept {
  auto result = SteamNetworkingMessages()->SendMessageToUser(
      HOST_ID, msg, length, k_nSteamNetworkingSend_Reliable, channel);

  Multiplayer::CleanUpMessage(channel, msg);

  if (result != k_EResultOK) {
    Multiplayer::CloseMultiplayer();
  }
}
static void PollPackets() noexcept {

}
inline bool connected = false;
inline int client_id = 0;
static void init(const char* ip) noexcept {
  connected = false;
  client_id = -1;
  auto send_data = new uint8_t[3 + PLAYER_NAME.size() + 1];
  send_data[0] = (uint8_t)CURRENT_ZONE;
  send_data[0] = (uint8_t)PLAYER_X;
  send_data[0] = (uint8_t)PLAYER_Y;
  transcribe_string_into_uint8(3, PLAYER_NAME, send_data);
  NBN_GameClient_Init(MG2_NET_IDENTIFIER, ip, MG2_PORT, false, send_data);
  if (NBN_GameClient_Start() < 0) {
    Log(NET_LOG_ERROR, "Failed to start client");
  }
  PROJECTILES.clear();
  MONSTERS.clear();
  delete[] send_data;
  MP_TYPE = MultiplayerType::CLIENT;
}
static void send_packets() noexcept {
  if (NBN_GameClient_SendPackets() < 0) {
    Log(NET_LOG_ERROR, "Failed to send packets. Exit");
  }
}
static void HandleConnection() noexcept {
  NBN_Stream* rs = NBN_GameClient_GetAcceptDataReadStream();

  int x = 0, y = 0, id = 0;
  NBN_SerializeInt(rs, x, 0, 24000);
  NBN_SerializeInt(rs, y, 0, 24000);
  NBN_SerializeInt(rs, id, 0, 4);
  client_id = id;
  //  OTHER_PLAYERS[0] = new NetPlayer({(float)x, (float)y}, "hey ", Zone(0), nullptr, 1);
  connected = true;
}
static void HandleDisconnection() {
  int code = NBN_GameClient_GetServerCloseCode();
  switch (code) {
    case MG2_HOST_CLOSED_GAMED:
      Log(NET_LOG_INFO, "Host closed game");
      break;
  }
  MP_TYPE = MultiplayerType::OFFLINE;
  for (auto& net_player : OTHER_PLAYERS) {
    delete net_player;
    net_player = nullptr;
  }
  NBN_GameClient_Disconnect();
  NBN_GameClient_Stop();
}
inline static void HandleProjectileUpdate(UDP_Projectile* data) noexcept {
  switch (data->p_type) {
    case FIRE_BALL: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, 250, 4, data->damage,
          HitType::ONE_HIT, {}, data->pov, {data->move_x, data->move_y}));
      break;
    }
    case FIRE_STRIKE: {
      PROJECTILES.emplace_back(new FireBall(
          {(float)data->x, (float)data->y}, !FRIENDLY_FIRE, 120, 2, data->damage,
          HitType::CONTINUOUS, {}, data->pov, {data->move_x, data->move_y}));
      break;
    }
  }
}
inline static void HandlePlayerPositionUpdate(UDP_PositionState* data) noexcept {
  for (uint_fast32_t j = 0; j < data->client_count; j++) {
    if (data->clients_pos[j].client_id == client_id) continue;
    if (OTHER_PLAYERS[j]) {
      OTHER_PLAYERS[j]->update_state(data->clients_pos[j].x, data->clients_pos[j].y);
    } else {
      // OTHER_PLAYERS[j] =
      //    new NetPlayer({(float)data->clients_pos[j].x, (float)data->clients_pos[j].y},
      //                "", CURRENT_ZONE, nullptr, 50);
    }
  }
  UDP_PositionState_Destroy(data);
}
inline static int receive_packet() noexcept {
  if (!connected) return 0;
  NBN_MessageInfo msg_info = NBN_GameClient_GetMessageInfo();
  switch (msg_info.type) {
    case UDP_PLAYER_STATE:
      HandlePlayerPositionUpdate((UDP_PositionState*)msg_info.data);
      break;

    case UDP_PROJECTILE:
      HandleProjectileUpdate((UDP_Projectile*)msg_info.data);
      break;
  }
  return 0;
}
inline static void poll_events() noexcept {
  NBN_GameClient_AddTime(MG2_TICK_TIME);
  while ((MP_EVENT_CODE = NBN_GameClient_Poll()) != NBN_NO_EVENT && connected) {
    switch (MP_EVENT_CODE) {
      case NBN_CONNECTED:
        HandleConnection();
        break;

      case NBN_DISCONNECTED:
        HandleDisconnection();
        return;

      case NBN_MESSAGE_RECEIVED:
        receive_packet();
        break;
    }
  }
}

}  // namespace Client
#endif  //MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
