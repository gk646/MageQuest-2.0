#ifndef MAGEQUEST_SRC_INCLUDES_H_
#define MAGEQUEST_SRC_INCLUDES_H_

/* |-----------------------------------------------------|
 * |                      STL                            |
 * |-----------------------------------------------------|
 */
#include <array>
#include <chrono>
#include <csignal>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
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

using namespace std::chrono_literals;
/* |-----------------------------------------------------|
 * |                         RAYLIB                      |
 * |-----------------------------------------------------|
 */
#define RAYLIB_IMPLEMENTATION
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(_WIN32) || defined(_WIN64)
#define NOGDICAPMASKS      // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES  // VK_*
#define NOWINMESSAGES      // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES        // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS       // SM_*
#define NOMENUS            // MF_*
#define NOICONS            // IDI_*
#define NOKEYSTATES        // MK_*
#define NOSYSCOMMANDS      // SC_*
#define NORASTEROPS        // Binary and Tertiary raster ops
#define NOSHOWWINDOW       // SW_*
#define OEMRESOURCE        // OEM Resource values
#define NOATOM             // Atom Manager routines
#define NOCLIPBOARD        // Clipboard routines
#define NOCOLOR            // Screen colors
#define NOCTLMGR           // Control and Dialog routines
#define NODRAWTEXT         // DrawText() and DT_*
#define NOGDI              // All GDI defines and routines
#define NOKERNEL           // All KERNEL defines and routines
#define NOUSER             // All USER defines and routines
//#define NONLS            // All NLS defines and routines
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions
#endif
/* |-----------------------------------------------------|
 * |                        STEAM                        |
 * |-----------------------------------------------------|
 */
#include <steam/steam_api.h>

/* |-----------------------------------------------------|
 * |                         SQLITE                      |
 * |-----------------------------------------------------|
 */
#include <sqlite3.h>
/* |-----------------------------------------------------|
 * |                      CXSTRUCTS                      |
 * |-----------------------------------------------------|
 */
#include "cxstructs/Geometry.h"
#include "cxstructs/HashMap.h"
#include "cxstructs/PriorityQueue.h"
#include "cxstructs/QuadTree.h"
#include <cxstructs/HashSet.h>
#include "cxstructs/vec.h"
#include "cxutil/cxtime.h"
using namespace cxstructs;

/* |-----------------------------------------------------|
 * |                  MAGE QUEST II                      |
 * |-----------------------------------------------------|
 */
#include "raylib-extension/DrawFunctions.h"
#include "system/CollisionDetection.h"
#include "system/Definitions.h"
#include "resources/Colors.h"
#include "system/Enums.h"
#include "system/GameSettings.h"
#include "system/GameSettings.h"
#include "resources/FontStorage.h"
#include "graphics/elements/LogoDrawer.h"
#include "system/Util.h"
#include "world/Map.h"
#include "gamestateio/loading/EntityResource.h"
#include "system/GlobalVariables.h"
#include "resources/SoundStorage.h"
#include "resources/TextureStorage.h"
#include "ui/game/DialogueRender.h"
#include "gameplay/Item.h"
#include "ui/elements/Window.h"
#include "gameplay/Talent.h"
#include "ui/StyleSheet.h"
#include "ui/player/elements/InventorySlot.h"
#include "ui/player/CharacterBag.h"
#include "gameobjects/Entity.h"
#include "gameobjects/Projectile.h"
#include "graphics/Lighting.h"
#include "multiplayer/Multiplayer.h"
#include "steam/SteamCallbacks.h"
#include "gameobjects/Player.h"
#include "graphics/WorldRender.h"
#include "gameplay/handlers/ItemDropHandler.h"
#include "gameobjects/Monster.h"
#include "gameobjects/NPC.h"
#include "world/WorldManager.h"
#include "multiplayer/menus/ServerBrowser.h"
#include "gamestateio/DataBaseHandler.h"
#include "ui/game/LoadingScreen.h"
#include "gamestateio/loading/LoadingUtil.h"
#include "system/BenchMark.h"
#include "ui/game/MiniMap.h"
#include "gamestateio/GameLoader.h"
#include "gamestateio/loading/loaders/GameSaver.h"
#include "ui/UIManager.h"
#include "Game.h"
#endif  //MAGEQUEST_SRC_INCLUDES_H_
