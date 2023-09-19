#ifndef MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
#define MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_

namespace Client {
//syncs the OTHER_PLAYER array when the lobby state changes
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
static void HandlePositionUpdates(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES];
  int numMsgs;
  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_PLAYER_POS_BROADCAST, pMessages,
                                               MP_MAX_MESSAGES)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_PlayerPositionBroadcast*)pMessages[i]->GetData();
      for (uint8_t j = 0; j < data->client_count; j++) {
        CSteamID steam_id = CSteamID(data->clients_pos[j].steam_id);
        if (steam_id != PLAYER_ID) {
          Multiplayer::get(steam_id)->update_state(data->clients_pos[j].x,
                                                   data->clients_pos[j].y);
        }
      }
      pMessages[i]->Release();
    }
  }
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
static void PollPackets() noexcept {
  auto pInterface = SteamNetworkingMessages();

  HandlePositionUpdates(pInterface);
}
}  // namespace Client
#endif  //MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
