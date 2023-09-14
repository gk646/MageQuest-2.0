#ifndef MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_

namespace Server {
inline static int connected_clients = 0;
inline static void RegisterMessages() noexcept {
  NBN_GameServer_RegisterMessage(UDP_PLAYER_POS_CLIENT,
                                 (NBN_MessageBuilder)UDP_PlayerPos_Client_Create,
                                 (NBN_MessageDestructor)UDP_PlayerPos_Client_Destroy,
                                 (NBN_MessageSerializer)UDP_PlayerPos_Client_Serialize);
  NBN_GameServer_RegisterMessage(UDP_PLAYER_STATE,
                                 (NBN_MessageBuilder)UDP_PositionState_Create,
                                 (NBN_MessageDestructor)UDP_PositionState_Destroy,
                                 (NBN_MessageSerializer)UDP_PositionState_Serialize);
  NBN_GameServer_RegisterMessage(UDP_PROJECTILE,
                                 (NBN_MessageBuilder)UDP_Projectile_Create,
                                 (NBN_MessageDestructor)UDP_Projectile_Destroy,
                                 (NBN_MessageSerializer)UDP_Projectile_Serialize);
}
static void init() {
  connected_clients = 0;
  NBN_GameServer_Init(MG2_NET_IDENTIFIER, MG2_PORT, false);
  RegisterMessages();
  if (NBN_GameServer_Start() < 0) {
    Log(NET_LOG_ERROR, "Failed to start the server");
  }
  MP_TYPE = MultiplayerType::SERVER;
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
      msg->client_count = connected_clients + 1;
      memcpy(msg->clients_pos, broad_cast,
             sizeof(UDP_PlayerPos_Update) * MG2_MAX_PLAYERS);
      NBN_GameServer_SendUnreliableMessageTo(net_player->connection, UDP_PLAYER_STATE,
                                             msg);
    }
  }
}
inline static void send_packets() noexcept {
  BroadCastGameState();
  NBN_GameServer_SendPackets();
}
static void HandleProjectileState(UDP_Projectile* data) noexcept {
  switch (data->p_type) {
    case FIRE_BALL: {
      PROJECTILES.emplace_back(new FireBall({data->x, data->y}, !FRIENDLY_FIRE, 250, 4,
                                            data->damage, HitType::ONE_HIT, {}, data->pov,
                                            {data->move_x, data->move_y}));
      break;
    }
    case FIRE_STRIKE: {
      PROJECTILES.emplace_back(new FireBall({data->x, data->y}, !FRIENDLY_FIRE, 120, 2,
                                            data->damage, HitType::CONTINUOUS, {},
                                            data->pov, {data->move_x, data->move_y}));
      break;
    }
  }
  for (auto net_player : OTHER_PLAYERS) {
    if (net_player) {
      auto prj = UDP_Projectile_Create();
      memcpy(prj, data, sizeof(UDP_Projectile));
      NBN_GameServer_SendReliableMessageTo(net_player->connection, UDP_PROJECTILE, prj);
    }
  }
  UDP_Projectile_Destroy(data);
}
static void HandlePositionUpdate(const NBN_MessageInfo& msg) noexcept {
  auto data = (UDP_PlayerPos*)msg.data;
  auto sender = OTHER_PLAYERS[msg.sender->id % MG2_MAX_CLIENTS];
  if (sender) {
    sender->update_state(data->x, data->y);
    sender->pos.x_ = data->x;
    sender->pos.y_ = data->y;
  }
  UDP_PlayerPos_Client_Destroy(data);
}
static int HandleReceiveMessage() noexcept {
  auto msg_info = NBN_GameServer_GetMessageInfo();
  switch (msg_info.type) {
    case UDP_PLAYER_POS_CLIENT: {
      HandlePositionUpdate(msg_info);
      break;
    }
    case UDP_PROJECTILE: {
      HandleProjectileState((UDP_Projectile*)msg_info.data);
      break;
    }
  }

  return 0;
}
inline static void HandleNewConnection() {
  if (connected_clients == MG2_MAX_CLIENTS) {
    Log(NET_LOG_INFO, "Connection rejected | Limit reached");
    NBN_GameServer_RejectIncomingConnectionWithCode(MG2_SERVER_BUSY);
    return;
  }

  auto connection = NBN_GameServer_GetIncomingConnection();
  auto client_data = NBN_GameServer_GetConnectionData(connection);

  int client_id = connection->id % MG2_MAX_CLIENTS;
  OTHER_PLAYERS[client_id] = new NetPlayer({(float)client_data[1], (float)client_data[2]},
                                           read_string_from_uint8(client_data, 3),
                                           Zone(client_data[0]), connection, client_id);

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
  connected_clients++;
  Log(NET_LOG_INFO, "Client accepted (ID: %d)", connection->id);
}
inline static void HandleClientDisconnect() noexcept {
  NBN_Connection* cli_conn = NBN_GameServer_GetDisconnectedClient();

  Log(NET_LOG_INFO, "Client has disconnected (id: %d)", cli_conn->id);

  delete OTHER_PLAYERS[cli_conn->id % MG2_MAX_CLIENTS];
  OTHER_PLAYERS[cli_conn->id % MG2_MAX_CLIENTS] = nullptr;
  connected_clients--;
  NBN_Connection_Destroy(cli_conn);
}
static void poll_events() noexcept {
  NBN_GameServer_AddTime(MG2_TICK_TIME);
  while ((MP_EVENT_CODE = NBN_GameServer_Poll()) != NBN_NO_EVENT && connected_clients > 0) {
    switch (MP_EVENT_CODE) {
      case NBN_NEW_CONNECTION:
        HandleNewConnection();
        break;

      case NBN_CLIENT_DISCONNECTED:
        HandleClientDisconnect();
        break;

      case NBN_CLIENT_MESSAGE_RECEIVED:
        HandleReceiveMessage();
        break;
    }
  }
}

}  // namespace Server

#endif  //MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
