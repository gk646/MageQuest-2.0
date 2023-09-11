#ifndef MAGEQUEST_SRC_INCLUDES_H_
#define MAGEQUEST_SRC_INCLUDES_H_

#ifndef M_PI
#define M_PI 3.1415926
#endif

//stdlib
#include <array>
#include <chrono>
#include <csignal>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>
#include <regex>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

//raylib
#define RAYLIB_IMPLEMENTATION
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//SQlite
#include <sqlite3.h>
inline std::string ASSET_PATH = "../res/";


//nbnet
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


//cxstructs
#include "cxstructs/Geometry.h"
#include "cxstructs/vec.h"
#include "cxutil/cxtime.h"
using namespace cxstructs;
//Mage Quest
#include "system/Definitions.h"

#include "gamestateio/loading/EntityResource.h"
#include "resources/Colors.h"
#include "resources/FontStorage.h"
#include "resources/SoundStorage.h"
#include "resources/TextureStorage.h"
#include "system/Enums.h"
#include "system/GameSettings.h"
#include "system/Util.h"
#include "ui/StyleSheet.h"

#include "gameplay/Item.h"

#include "gameplay/Stats.h"
#include "gameplay/Talent.h"
#include "ui/player/elements/InventorySlot.h"

#include "world/maps/Map.h"

#include "gameplay/StatusEffect.h"
#include "gameplay/handlers/StatusEffectHandler.h"
#include "graphics/GifDrawer.h"
#include "ui/Window.h"

#include "system/GlobalVariables.h"

//after global variables
#include "gamestateio/DataBaseHandler.h"
#include "ui/game/LoadingScreen.h"
#include "ui/player/CharacterPanel.h"

#include "gameplay/handlers/ItemDropHandler.h"
#include "gamestateio/loading/LoadingUtil.h"
#include "graphics/WorldRender.h"
#include "system/BenchMark.h"
#include "ui/game/MiniMap.h"

#include "ui/menus/SettingsMenu.h"

#include "gamestateio/GameLoader.h"
#include "gamestateio/GameSaver.h"

#include "ui/PlayerUI.h"

#include "ui/menus/GameMenu.h"
#include "ui/menus/MainMenu.h"

#include "ui/UIManager.h"

#include "Game.h"

#endif  //MAGEQUEST_SRC_INCLUDES_H_
