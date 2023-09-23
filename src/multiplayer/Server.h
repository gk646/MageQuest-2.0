#ifndef MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
#define MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_

namespace Server {
static void SynchronizeMonsters(const SteamNetworkingIdentity& identity) noexcept;

static void SendMsgToAllUsers(uint8_t channel, const void* msg,
                              uint32_t length) noexcept {
  for (auto p : OTHER_PLAYERS) {
    if (p) {
      auto res = SteamNetworkingMessages()->SendMessageToUser(
          p->identity, msg, length, k_nSteamNetworkingSend_Reliable, channel);
      if (res != k_EResultOK) {
        Multiplayer::remove(p->identity.GetSteamID());
      }
    }
  }
  Multiplayer::CleanUpMessage(channel, msg);
}
static void SendMsgToUser(const SteamNetworkingIdentity& identity, uint8_t channel,
                          const void* msg, uint32_t length) noexcept {
  auto res = SteamNetworkingMessages()->SendMessageToUser(
      identity, msg, length, k_nSteamNetworkingSend_Reliable, channel);
  if (res != k_EResultOK) {
    Multiplayer::remove(identity.GetSteamID());
  }

  Multiplayer::CleanUpMessage(channel, msg);
}

static void SendMsgToAllUsersExcept(uint8_t channel, const void* msg, uint32_t length,
                                    const SteamNetworkingIdentity& blacklist) noexcept {
  for (auto p : OTHER_PLAYERS) {
    if (p && p->identity != blacklist) {
      auto res = SteamNetworkingMessages()->SendMessageToUser(
          p->identity, msg, length, k_nSteamNetworkingSend_Reliable, channel);
      if (res != k_EResultOK) {
        Multiplayer::remove(p->identity.GetSteamID());
      }
    }
  }
  Multiplayer::CleanUpMessage(channel, msg);
}
static void HandlePositionUpdates(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES_POS];
  int numMsgs;

  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_PLAYER_POS_UPDATE, pMessages,
                                               MP_MAX_MESSAGES_POS)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_PlayerPos*)pMessages[i]->GetData();
      NetPlayer* p;
      if ((p = Multiplayer::get(pMessages[i]->m_identityPeer.GetSteamID())) != nullptr) {
        p->update_state(data->x, data->y);
      }
      pMessages[i]->Release();
    }
  }
}
static void HandleProjectileState(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES_MONSTER];
  int numMsgs;

  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_PROJECTILE, pMessages,
                                               MP_MAX_MESSAGES_MONSTER)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_Projectile*)pMessages[i]->GetData();
      Multiplayer::HandleProjectile(data,Multiplayer::get(pMessages[i]->m_identityPeer));
      auto new_data = new UDP_Projectile();
      memcpy(new_data, data,
             sizeof(UDP_Projectile));  //SendMsgToAllUsers cleans up the data
      SendMsgToAllUsersExcept(UDP_PROJECTILE, new_data, sizeof(UDP_Projectile),
                              pMessages[i]->m_identityPeer);
      pMessages[i]->Release();
    }
  }
}
inline static void BroadCastPlayerPositions() noexcept {
  UDP_PlayerPosNamed pos_arr[MG2_MAX_PLAYERS];

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
inline static void BroadCastMonsterUpdates() noexcept;
inline static void BroadCastGameState() noexcept {
  BroadCastPlayerPositions();
  BroadCastMonsterUpdates();
}
static void PollPackets() noexcept {
  auto pInterface = SteamNetworkingMessages();

  HandlePositionUpdates(pInterface);
  HandleProjectileState(pInterface);
}
}  // namespace Server

#endif  //MAGEQUEST_SRC_MULTIPLAYER_SERVER_H_
