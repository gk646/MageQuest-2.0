#ifndef MAGEQUEST_SRC_STEAM_CALLBACKS_H_
#define MAGEQUEST_SRC_STEAM_CALLBACKS_H_
namespace Steam {
class SteamCallbacks {
 private:
  STEAM_CALLBACK(SteamCallbacks, OnGameOverlayActivated, GameOverlayActivated_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyCreated, LobbyCreated_t);

  STEAM_CALLBACK(SteamCallbacks, OnLobbyEnter, LobbyEnter_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyDataUpdated, LobbyDataUpdate_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyChatUpdate, LobbyChatUpdate_t);
  STEAM_CALLBACK(SteamCallbacks, OnLobbyJoinRequest, GameLobbyJoinRequested_t);
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
  if(pParam->m_eResult == k_EResultOK){
    LOBBY_ID = pParam->m_ulSteamIDLobby;
    std::cout<< "success" << std::endl;
  }
}

void SteamCallbacks::OnLobbyEnter(LobbyEnter_t* pCallback) {
  if (pCallback->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess) {

  }
}

void SteamCallbacks::OnLobbyDataUpdated(LobbyDataUpdate_t* pCallback) {

}

void SteamCallbacks::OnLobbyChatUpdate(LobbyChatUpdate_t* pCallback) {
  if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeEntered) {
    Multiplayer::add_netplayer(pCallback->m_ulSteamIDUserChanged);
  } else if (pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeLeft ||
             pCallback->m_rgfChatMemberStateChange & k_EChatMemberStateChangeDisconnected) {
    Multiplayer::remove_netplayer(pCallback->m_ulSteamIDUserChanged);
  }
}


void SteamCallbacks::OnLobbyJoinRequest(GameLobbyJoinRequested_t* pParam) {
  SteamMatchmaking()->JoinLobby(pParam->m_steamIDLobby);
}
}  // namespace Steam
#endif  //MAGEQUEST_SRC_STEAM_CALLBACKS_H_
