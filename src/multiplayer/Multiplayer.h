#ifndef MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
#define MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_

#include "MessageTypes.h"
#include "NetPlayer.h"

namespace Multiplayer {
inline static void HandleProjectile(UDP_Projectile* data) noexcept;
inline static int count() noexcept {
  int ret = 0;
  for (auto p : OTHER_PLAYERS) {
    if (p) {
      ret++;
    }
  }
  return ret;
}
inline static NetPlayer* get(CSteamID steam_id) noexcept {
  for (auto& np : OTHER_PLAYERS) {
    if (np && np->identity.GetSteamID() == steam_id) {
      return np;
    }
  }
  return nullptr;
}
inline static void add(CSteamID steam_id) noexcept {
  if (!get(steam_id)) {
    for (auto& np : OTHER_PLAYERS) {
      if (!np) {
        np = new NetPlayer({150, 150}, Zone::Woodland_Edge, steam_id, 0);
        break;
      }
    }
  }
}
inline static void remove(CSteamID steam_id) noexcept {
  for (auto& np : OTHER_PLAYERS) {
    if (np && np->identity.GetSteamID() == steam_id) {
      delete np;
      np = nullptr;
      break;
    }
  }
}
inline static void CloseMultiplayer() noexcept {
  for (auto& np : OTHER_PLAYERS) {
    delete np;
    np = nullptr;
  }
  SteamMatchmaking()->LeaveLobby(LOBBY_ID);
  MP_TYPE = MultiplayerType::OFFLINE;
}
inline static void CleanUpMessage(uint8_t channel, const void* msg) noexcept {
  switch (channel) {
    case UDP_PROJECTILE:
      delete (UDP_Projectile*)msg;
      break;
    case UDP_PLAYER_POS_BROADCAST:
      delete (UDP_PlayerPositionBroadcast*)msg;
      break;
  }
}
}  // namespace Multiplayer

#include "Client.h"
#include "Server.h"
#include "menus/HostMenu.h"

namespace Multiplayer {
inline static SteamNetConnectionRealTimeStatus_t connect_status =
    SteamNetConnectionRealTimeStatus_t();
inline static void UDP_SEND_PROJECTILE(ProjectileType type, int16_t x, int16_t y,
                                       float pov, float mx, float my, float dmg) {
  if (MP_TYPE == MultiplayerType::CLIENT) {
    Client::SendMsgToHost(UDP_PROJECTILE,
                          new UDP_Projectile(type, x, y, pov, mx, my, dmg),
                          sizeof(UDP_Projectile));
  } else if (MP_TYPE == MultiplayerType::SERVER) {
    Server::SendMsgToAllUsers(UDP_PROJECTILE,
                              new UDP_Projectile(type, x, y, pov, mx, my, dmg),
                              sizeof(UDP_Projectile));
  }
}
inline static void UDP_SEND_POSITION(uint16_t x, uint16_t y) {
  if (MP_TYPE == MultiplayerType::CLIENT) {
    Client::SendMsgToHost(UDP_PLAYER_POS_UPDATE, new UDP_PlayerPos(x, y),
                          sizeof(UDP_PlayerPos));
  }
}
inline static void PollPackets() noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    Server::PollPackets();
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    Client::PollPackets();
  }
}
inline static void BroadCastGameState() noexcept {
  Server::BroadCastGameState();
}
inline static void draw_stats(char* buffer) noexcept {
  if (MP_TYPE == MultiplayerType::SERVER) {
    float download = 0;
    float upload = 0;
    for (auto np : OTHER_PLAYERS) {
      if (np) {
        SteamNetworkingMessages()->GetSessionConnectionInfo(np->identity, nullptr,
                                                            &connect_status);
        download += connect_status.m_flInBytesPerSec;
        upload += connect_status.m_flOutBytesPerSec;
      }
    }
    sprintf(buffer, "Upload: %.1f Bps", upload);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.2, 20, GREEN);
    sprintf(buffer, "Download: %.1f Bps", download);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.25, 20, GREEN);
  } else if (MP_TYPE == MultiplayerType::CLIENT) {
    SteamNetworkingMessages()->GetSessionConnectionInfo(HOST_ID, nullptr,
                                                        &connect_status);
    sprintf(buffer, "Upload: %.1f Bps", connect_status.m_flOutBytesPerSec);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.2, 20, GREEN);
    sprintf(buffer, "Download: %.1f Bps", connect_status.m_flInBytesPerSec);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.25, 20, GREEN);
    sprintf(buffer, "Ping: %d ms", connect_status.m_nPing);
    DrawTextR(buffer, SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.35, 20, GREEN);
  }
}

}  // namespace Multiplayer
#endif  //MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
