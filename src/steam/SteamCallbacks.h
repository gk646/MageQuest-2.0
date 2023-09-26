#ifndef MAGEQUEST_SRC_STEAM_STEAMCALLBACKS_H_
#define MAGEQUEST_SRC_STEAM_STEAMCALLBACKS_H_
namespace Steam {
class SteamCallbacks {
 private:
  STEAM_CALLBACK(SteamCallbacks, OnGameOverlayActivated, GameOverlayActivated_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyCreated, LobbyCreated_t);

  STEAM_CALLBACK(SteamCallbacks, OnLobbyEnter, LobbyEnter_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyDataUpdated, LobbyDataUpdate_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyChatUpdate, LobbyChatUpdate_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyJoinRequest, GameLobbyJoinRequested_t);
  STEAM_CALLBACK(SteamCallbacks, OnConnectionRequested,
                 SteamNetworkingMessagesSessionRequest_t);
  STEAM_CALLBACK(SteamCallbacks, OnConnectionFailed,
                 SteamNetworkingMessagesSessionFailed_t);
  STEAM_CALLBACK(SteamCallbacks, OnGameJoinRequest, GameRichPresenceJoinRequested_t);
};
SteamCallbacks STEAM_CALLBACKS = SteamCallbacks();

void SteamCallbacks::OnGameOverlayActivated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    if (GAME_STATE == GameState::Game) {
      GAME_STATE = GameState::GameMenu;
    }
  } else {
    if (GAME_STATE == GameState::GameMenu) {
      GAME_STATE = GameState::Game;
    }
  }
}
void SteamCallbacks::OnLobbyCreated(LobbyCreated_t* pParam) {
  if (pParam->m_eResult == k_EResultOK) {
    MP_TYPE = MultiplayerType::SERVER;
  }
}
void SteamCallbacks::OnLobbyEnter(LobbyEnter_t* pCallback) {
  if (pCallback->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess) {
    LOBBY_ID = pCallback->m_ulSteamIDLobby;
    const char* inGameName = SteamFriends()->GetPersonaName();  //TODO player name
    SteamMatchmaking()->SetLobbyMemberData(LOBBY_ID, "InGameName", inGameName);

    if (MP_TYPE == MultiplayerType::CLIENT) {
      Client::SyncPlayers();
      HOST_ID = SteamNetworkingIdentity();
      HOST_ID.SetSteamID(SteamMatchmaking()->GetLobbyOwner(LOBBY_ID));
    }
  } else if (pCallback->m_EChatRoomEnterResponse != k_EChatRoomEnterResponseSuccess) {
    MP_TYPE = MultiplayerType::OFFLINE;
  }
}
void SteamCallbacks::OnLobbyDataUpdated(LobbyDataUpdate_t* pCallback) {}
void SteamCallbacks::OnLobbyChatUpdate(LobbyChatUpdate_t* pCallback) {
  if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeEntered) {
    Multiplayer::add(pCallback->m_ulSteamIDUserChanged);
    if (MP_TYPE == MultiplayerType::SERVER) {
      Server::SynchronizeMonsters(
          Multiplayer::get(pCallback->m_ulSteamIDUserChanged)->identity);
    }
  } else if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeLeft ||
             pCallback->m_rgfChatMemberStateChange &
                 k_EChatMemberStateChangeDisconnected) {
    Multiplayer::remove(pCallback->m_ulSteamIDUserChanged);
  }
}
void SteamCallbacks::OnLobbyJoinRequest(GameLobbyJoinRequested_t* pParam) {
  SteamMatchmaking()->JoinLobby(pParam->m_steamIDLobby);
  MP_TYPE = MultiplayerType::CLIENT;
  PROJECTILES.clear();
  MONSTERS.clear();
}
void SteamCallbacks::OnConnectionRequested(
    SteamNetworkingMessagesSessionRequest_t* pParam) {
  //TODO enable public lobbies
  CSteamID steamIDFriend(pParam->m_identityRemote.GetSteamID64());
  if (SteamFriends()->HasFriend(steamIDFriend, k_EFriendFlagImmediate)) {
    SteamNetworkingMessages()->AcceptSessionWithUser(pParam->m_identityRemote);
  } else {
    SteamNetworkingMessages()->CloseSessionWithUser(pParam->m_identityRemote);
  }
}
void SteamCallbacks::OnConnectionFailed(SteamNetworkingMessagesSessionFailed_t* pParam) {
  Multiplayer::remove(pParam->m_info.m_identityRemote.GetSteamID());
}
void SteamCallbacks::OnGameJoinRequest(GameRichPresenceJoinRequested_t* pParam) {
  auto friend_id = pParam->m_steamIDFriend;
  std::cout << pParam->m_rgchConnect << std::endl;
}
}  // namespace Steam
#endif  //MAGEQUEST_SRC_STEAM_STEAMCALLBACKS_H_
