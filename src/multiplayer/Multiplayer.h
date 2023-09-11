#ifndef MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
#define MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_

#include "Client.h"
#include "Server.h"


namespace Multiplayer {

inline static void send_event(UDP_MSG_TYPE event, void* data) noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    for (uint_fast32_t i = 0; i < 4; i++) {
      if (nbn_game_server.clients->connections[i]) {
        NBN_GameServer_SendReliableMessageTo(nbn_game_server.clients->connections[i],
                                             event, data);
      }
    }
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    NBN_GameClient_SendReliableMessage(event,data);
  }
}

inline static void poll_events() noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    Server::poll_events();
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    Client::poll_events();
  }
}

inline static void send_packets() noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    Server::send_packets();
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    Client::send_packets();
  }
}
inline static void close_mp() noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    NBN_GameServer_Stop();
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    NBN_GameClient_Stop();
  }
}
}  // namespace Multiplayer
#endif  //MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
