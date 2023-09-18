#ifndef MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
#define MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_

#include "MessageTypes.h"

#include "NetPlayer.h"
#include "Client.h"
#include "Server.h"

#include "menus/HostMenu.h"

namespace Multiplayer {
inline static NetPlayer* get_netplayer(CSteamID steam_id) noexcept {
  for (auto& np : OTHER_PLAYERS) {
    if (np && np->steam_id == steam_id) {
      return np;
    }
  }
  return nullptr;
}
inline static void add_netplayer(CSteamID steam_id) noexcept {
  if (!get_netplayer(steam_id)) {
    for (auto& np : OTHER_PLAYERS) {
      if (!np) {
        np = new NetPlayer({150, 150}, Zone::Woodland_Edge, steam_id, 0);
        break;
      }
    }
  }
}
inline static void remove_netplayer(CSteamID steam_id) noexcept {
  for (auto& np : OTHER_PLAYERS) {
    if (np && np->steam_id == steam_id) {
      delete np;
      np = nullptr;
      break;
    }
  }
}

inline static NBN_ConnectionStats client_stats;
inline static NBN_GameServerStats server_stats;
#define SEND_UDP_PROJECTILE(type, x, y, p, xc, yc, dmg)       \
  Multiplayer::send_event(UDP_PROJECTILE,                     \
                          MP_TYPE == MultiplayerType::OFFLINE \
                              ? nullptr                       \
                              : new UDP_Projectile(type, x, y, p, xc, yc, dmg));

inline static void send_event(UDP_MSG_TYPE event, void* data) noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    if (event == UDP_PROJECTILE) {
      for (const auto net_player : OTHER_PLAYERS) {
        if (net_player) {
          auto prj = UDP_Projectile_Create();
          memcpy(prj, data, sizeof(UDP_Projectile));
          // NBN_GameServer_SendUnreliableMessageTo(net_player->connection, UDP_PROJECTILE,
          //  prj);
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
  MP_TYPE = MultiplayerType::OFFLINE;
  Server::connected_clients = 0;
  Client::connected = false;
  if (MP_TYPE == MultiplayerType::SERVER) {
    for (auto& net_player : OTHER_PLAYERS) {
      if (net_player) {
        // NBN_GameServer_CloseClientWithCode(net_player->connection, MG2_HOST_CLOSED_GAMED);
      }
    }
    NBN_GameServer_SendPackets();
    for (auto& net_player : OTHER_PLAYERS) {
      if (net_player) {
        //NBN_Connection_Destroy(net_player->connection);
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
