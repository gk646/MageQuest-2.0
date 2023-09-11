#ifndef MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
#define MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_

enum LogCode {
  NET_LOG_INFO,
  NET_LOG_ERROR,
  NET_LOG_DEBUG,
  NET_LOG_TRACE,
  NET_LOG_WARNING
};

static const char* log_type_strings[] = {"INFO", "ERROR", "DEBUG", "TRACE", "WARNING"};
inline static std::array<NetPlayer*, 3> OTHER_PLAYERS = {0};
#define NBN_LogInfo(...) Log(NET_LOG_INFO, __VA_ARGS__)
#define NBN_LogError(...) Log(NET_LOG_ERROR, __VA_ARGS__)
#define NBN_LogDebug(...) Log(NET_LOG_DEBUG, __VA_ARGS__)
#define NBN_LogTrace(...) Log(NET_LOG_TRACE, __VA_ARGS__)
#define NBN_LogWarning(...) Log(NET_LOG_WARNING, __VA_ARGS__)

inline static void Log(int type, const char* fmt, ...) {
  if(type == 3) return;
  va_list args;

  va_start(args, fmt);

  printf("[%s] ", log_type_strings[type]);
  vprintf(fmt, args);
  printf("\n");

  va_end(args);
}

#define NBNET_IMPL
#include "nbnet.h"
#include "udp.h"

#include "MessageTypes.h"

#include "Client.h"
#include "Server.h"

#include "menus/ServerBrowser.h"

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
