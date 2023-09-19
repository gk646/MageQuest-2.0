#ifndef MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_

namespace Server {
static void SendMsgToAllUsers(uint8_t channel, const void* msg, uint32_t length) {
  for (auto p : OTHER_PLAYERS) {
    if (p) {
      auto res = SteamNetworkingMessages()->SendMessageToUser(
          p->identity, msg, length, k_nSteamNetworkingSend_Reliable, channel);
      if (res != k_EResultOK) {
        std::cout<< "hey" << std::endl;
        //Multiplayer::remove(p->identity.GetSteamID());
      }
    }
  }
  Multiplayer::CleanUpMessage(channel, msg);
}
static void HandlePositionUpdates(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES];
  int numMsgs;

  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_PLAYER_POS_UPDATE, pMessages,
                                               MP_MAX_MESSAGES)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_PlayerPos*)pMessages[i]->GetData();
      Multiplayer::get(pMessages[i]->m_identityPeer.GetSteamID())
          ->update_state(data->x, data->y);
      pMessages[i]->Release();
    }
  }
}
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
inline static void BroadCastGameState() noexcept {
  UDP_PlayerPos_Update pos_arr[MG2_MAX_PLAYERS];

  pos_arr[0].steam_id = SteamUser()->GetSteamID().ConvertToUint64();
  pos_arr[0].x = static_cast<uint16_t>(PLAYER_X);
  pos_arr[0].y = static_cast<uint16_t>(PLAYER_Y);
  int i = 1;
  for (auto np : OTHER_PLAYERS) {
    if (np) {
      pos_arr[i].steam_id = np->identity.GetSteamID().ConvertToUint64();
      pos_arr[i].x = static_cast<uint16_t>(np->pos.x_);
      pos_arr[i].y = static_cast<uint16_t>(np->pos.y_);
      i++;
    }
  }
  auto msg = new UDP_PlayerPositionBroadcast();
  msg->client_count = i;
  memcpy(msg->clients_pos, pos_arr, sizeof(pos_arr));
  SendMsgToAllUsers(UDP_PLAYER_POS_BROADCAST, msg, sizeof(UDP_PlayerPositionBroadcast));
}
static void PollPackets() noexcept {
  auto pInterface = SteamNetworkingMessages();

  HandlePositionUpdates(pInterface);
}
}  // namespace Server

#endif  //MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
