#ifndef MAGEQUEST_SRC_SYSTEM_NBNET_H_
#define MAGEQUEST_SRC_SYSTEM_NBNET_H_
enum LogCode {
  NET_LOG_INFO,
  NET_LOG_ERROR,
  NET_LOG_DEBUG,
  NET_LOG_TRACE,
  NET_LOG_WARNING
};
class NetPlayer;
static const char* log_type_strings[] = {"INFO", "ERROR", "DEBUG", "TRACE", "WARNING"};
inline static std::array<NetPlayer*, 3> OTHER_PLAYERS = {0};
#define NBN_LogInfo(...) Log(NET_LOG_INFO, __VA_ARGS__)
#define NBN_LogError(...) Log(NET_LOG_ERROR, __VA_ARGS__)
#define NBN_LogDebug(...) Log(NET_LOG_DEBUG, __VA_ARGS__)
#define NBN_LogTrace(...) Log(NET_LOG_TRACE, __VA_ARGS__)
#define NBN_LogWarning(...) Log(NET_LOG_WARNING, __VA_ARGS__)

inline static void Log(int type, const char* fmt, ...) {
  if (type == 3)return;

  va_list args;

  va_start(args, fmt);

  printf("[%s] ", log_type_strings[type]);
  vprintf(fmt, args);
  printf("\n");

  va_end(args);
}

#define NBNET_IMPL
#include <nbnet.h>
#include <udp.h>

#endif  //MAGEQUEST_SRC_SYSTEM_NBNET_H_
