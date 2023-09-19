#ifndef MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_

namespace Server {
static void SendMsgToAllUsers(uint8_t channel, const void* msg, uint32_t length) {
  for (auto p : OTHER_PLAYERS) {
    if (p) {
      SteamNetworkingMessages()->SendMessageToUser(
          p->identity, msg, length, k_nSteamNetworkingSend_Reliable, channel);
    }
  }

}
static void HandlePositionUpdates(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES];
  int numMsgs =
      api->ReceiveMessagesOnChannel(UDP_PLAYER_POS_CLIENT, pMessages, MP_MAX_MESSAGES);
  for (int i = 0; i < numMsgs; ++i) {
    auto data = (UDP_PlayerPos*)pMessages[i]->GetData();
    Multiplayer::get(pMessages[i]->m_identityPeer.GetSteamID())
        ->update_state(data->x, data->y);
    pMessages[i]->Release();
  }
}
inline static void BroadCastGameState() noexcept {
  UDP_PlayerPos_Update broad_cast[MG2_MAX_PLAYERS];
  int i = 1;
  broad_cast[0].client_id = 3;
  broad_cast[0].x = PLAYER_X;
  broad_cast[0].y = PLAYER_Y;
  for (auto net_player : OTHER_PLAYERS) {
    if (net_player) {
      broad_cast[i].client_id = net_player->client_id;
      broad_cast[i].x = net_player->pos.x_;
      broad_cast[i].y = net_player->pos.y_;
      i++;
    }
  }
  for (auto net_player : OTHER_PLAYERS) {
    if (net_player) {
      auto msg = UDP_PositionState_Create();
      //msg->client_count = connected_clients + 1;
      memcpy(msg->clients_pos, broad_cast,
             sizeof(UDP_PlayerPos_Update) * MG2_MAX_PLAYERS);
      //      NBN_GameServer_SendUnreliableMessageTo(net_player->connection, UDP_PLAYER_STATE,
      //                                     msg);
    }
  }
}
inline static void send_packets() noexcept {}
static void HandleProjectileState(UDP_Projectile* data) noexcept {
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
  for (auto net_player : OTHER_PLAYERS) {
    if (net_player) {
      // NBN_GameServer_SendReliableMessageTo(net_player->connection, UDP_PROJECTILE, prj);
    }
  }
}
inline static void HandleNewConnection() {
  if (1 == MG2_MAX_CLIENTS) {
    Log(NET_LOG_INFO, "Connection rejected | Limit reached");
    NBN_GameServer_RejectIncomingConnectionWithCode(MG2_SERVER_BUSY);
    return;
  }

  auto connection = NBN_GameServer_GetIncomingConnection();
  auto client_data = NBN_GameServer_GetConnectionData(connection);

  int client_id = connection->id % MG2_MAX_CLIENTS;
  //OTHER_PLAYERS[client_id] = new NetPlayer({(float)client_data[1], (float)client_data[2]},
  //                                         read_string_from_uint8(client_data, 3),
  //                                         Zone(client_data[0]), connection, client_id);

  auto stream = NBN_GameServer_GetConnectionAcceptDataWriteStream(connection);
  int player_x = (int)PLAYER_X;
  int player_y = (int)PLAYER_Y;
  NBN_SerializeInt(stream, player_x, 0, 24000);
  NBN_SerializeInt(stream, player_y, 0, 24000);
  NBN_SerializeInt(stream, client_id, 0, 4);

  NBN_GameServer_AcceptIncomingConnection();
  // NBN_GameServer_SendReliableMessageTo(
  //  connection, UDP_PLAYER_NAME,
  //   new UDP_PlayerName(PLAYER_NAME.size() + 1, const_cast<char*>(PLAYER_NAME.c_str())));
  Log(NET_LOG_INFO, "Client accepted (ID: %d)", connection->id);
}
inline static void HandleClientDisconnect() noexcept {
  NBN_Connection* cli_conn = NBN_GameServer_GetDisconnectedClient();

  Log(NET_LOG_INFO, "Client has disconnected (id: %d)", cli_conn->id);

  delete OTHER_PLAYERS[cli_conn->id % MG2_MAX_CLIENTS];
  OTHER_PLAYERS[cli_conn->id % MG2_MAX_CLIENTS] = nullptr;
  NBN_Connection_Destroy(cli_conn);
}
static void PollPackets() noexcept {
  auto pInterface = SteamNetworkingMessages();

  HandlePositionUpdates(pInterface);
}

}  // namespace Server

#endif  //MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
