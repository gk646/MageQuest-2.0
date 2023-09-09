#ifndef MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
#define MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_

#include <cstdarg>
#include <cstdio>

void nbnet_logging(const char* format, ...)
{
  va_list args;
  va_start(args, format);

  // Your logging logic here, for example:
  vprintf(format, args);

  va_end(args);
}

#define NBN_LogInfo(...) nbnet_logging("[INFO] " __VA_ARGS__)
#define NBN_LogError(...) nbnet_logging("[ERROR] " __VA_ARGS__)
#define NBN_LogDebug(...) nbnet_logging("[DEBUG] " __VA_ARGS__)
#define NBN_LogTrace(...) nbnet_logging("[TRACE] " __VA_ARGS__)

#define NBNET_IMPL

#include "nbnet/nbnet.h"

#include "nbnet/udp.h"


struct MultiPlayer {


};
#endif  //MAGE_QUEST_SRC_NETCODE_MULTIPLAYER_H_
