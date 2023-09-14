#ifndef MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
#define MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_

#include "MessageTypes.h"

#include "NetPlayer.h"
#include "Client.h"
#include "Server.h"

namespace Multiplayer {
inline static NBN_ConnectionStats client_stats;
inline static NBN_GameServerStats server_stats;
inline static void send_event(UDP_MSG_TYPE event, void* data) noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    if (event == UDP_PROJECTILE) {
      for (const auto net_player : OTHER_PLAYERS) {
        if (net_player) {
          auto prj = UDP_Projectile_Create();
          memcpy(prj,data,sizeof(UDP_Projectile));
          NBN_GameServer_SendReliableMessageTo(net_player->connection, UDP_PROJECTILE,
                                               prj);
        }
      }
      UDP_Projectile_Destroy((UDP_Projectile*)data);
    }
  } else if (MP_TYPE == MultiplayerType::CLIENT && Client::connected) {
    NBN_GameClient_SendUnreliableMessage(event, data);
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
    server_stats = NBN_GameServer_GetStats();
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    Client::send_packets();
    client_stats = NBN_GameClient_GetStats();
  }
}
inline static void close_mp() noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    for (auto& net_player : OTHER_PLAYERS) {
      if (net_player) {
        NBN_GameServer_CloseClientWithCode(net_player->connection, MG2_HOST_CLOSED_GAMED);
      }
    }
    NBN_GameServer_SendPackets();
    for (auto& net_player : OTHER_PLAYERS) {
      if (net_player) {
        NBN_Connection_Destroy(net_player->connection);
        delete net_player;
        net_player = nullptr;
      }
    }
    GameServer_RemoveClosedClientConnections();
    NBN_GameServer_Stop();
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    for (auto& net_player : OTHER_PLAYERS) {
      delete net_player;
      net_player = nullptr;
    }
    NBN_GameClient_Disconnect();
    NBN_GameClient_Stop();
  }
  MP_TYPE = MultiplayerType::OFFLINE;
}
inline static void draw_stats(char* buffer) noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    sprintf(buffer, "Upload: %.1f Bps", server_stats.upload_bandwidth);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.2, 20, GREEN);
    sprintf(buffer, "Download: %.1f Bps", server_stats.download_bandwidth);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.25, 20, GREEN);
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    sprintf(buffer, "Upload: %.1f Bps", client_stats.upload_bandwidth);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.2, 20, GREEN);
    sprintf(buffer, "Download: %.1f Bps", client_stats.download_bandwidth);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.25, 20, GREEN);
    sprintf(buffer, "Packet loss: %d %%", (int)client_stats.packet_loss);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.3, 20, GREEN);
    sprintf(buffer, "Ping: %d ms", (int)client_stats.ping);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.35, 20, GREEN);
  }
}

}  // namespace Multiplayer
#endif  //MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
