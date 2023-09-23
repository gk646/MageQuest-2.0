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
      Multiplayer::add(memberId);
      Multiplayer::get(memberId)->name = inGameName;
    }
  }
  Multiplayer::CONNECTED = true;
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
inline static void UpdateMonsters(UDP_MonsterUpdate* data) noexcept;
static void HandlePositionUpdates(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES_POS];
  int numMsgs;
  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_PLAYER_POS_BROADCAST, pMessages,
                                               MP_MAX_MESSAGES_POS)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_PlayerPositionBroadcast*)pMessages[i]->GetData();
      for (uint8_t j = 0; j < data->client_count; j++) {
        auto steam_id = CSteamID(data->clients_pos[j].steam_id);
        if (steam_id != PLAYER_ID) {
          Multiplayer::get(steam_id)->update_state(data->clients_pos[j].x,
                                                   data->clients_pos[j].y);
        }
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
      Multiplayer::HandleProjectile(data, nullptr);
      pMessages[i]->Release();
    }
  }
}
static void HandleMonsterSpawns(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES];
  int numMsgs;

  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_MONSTER_SPAWN, pMessages,
                                               MP_MAX_MESSAGES)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_MonsterSpawn*)pMessages[i]->GetData();
      Multiplayer::HandleMonsterSpawn(data);
      pMessages[i]->Release();
    }
  }
}
static void HandleMonsterUpdates(ISteamNetworkingMessages* api) noexcept {
  SteamNetworkingMessage_t* pMessages[MP_MAX_MESSAGES_MONSTER];
  int numMsgs;

  if ((numMsgs = api->ReceiveMessagesOnChannel(UDP_MONSTER_UPDATE, pMessages,
                                               MP_MAX_MESSAGES_MONSTER)) > 0) {
    for (int i = 0; i < numMsgs; ++i) {
      auto data = (UDP_MonsterUpdate*)pMessages[i]->GetData();
      UpdateMonsters(data);
      pMessages[i]->Release();
    }
  }
}
static void PollPackets() noexcept {
  auto pInterface = SteamNetworkingMessages();

  HandlePositionUpdates(pInterface);
  HandleProjectileState(pInterface);
  HandleMonsterSpawns(pInterface);
  HandleMonsterUpdates(pInterface);
}
}  // namespace Client
#endif  //MAGEQUEST_SRC_MULTIPLAYER_CLIENT_H_
