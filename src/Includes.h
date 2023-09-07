#ifndef MAGEQUEST_SRC_INCLUDES_H_
#define MAGEQUEST_SRC_INCLUDES_H_

//stdlib
#include <array>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

//raylib
#define RAYLIB_IMPLEMENTATION
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//SFML
//#include <SFML/Network.hpp>

//SQlite
#include <sqlite3.h>
inline std::string ASSET_PATH = "../res/";

//cxstructs
#include "cxstructs/Geometry.h"
#include "cxstructs/vec.h"
#include "cxutil/cxtime.h"
using namespace cxstructs;
//Mage Quest
#include "resources/Colors.h"
#include "resources/FontStorage.h"
#include "resources/SoundStorage.h"
#include "system/Definitions.h"
#include "system/Enums.h"
#include "system/GameSettings.h"
#include "system/Util.h"
#include "ui/StyleSheet.h"

#include "gameplay/Item.h"

#include "gameplay/Stats.h"
#include "gameplay/Talent.h"
#include "world/maps/Map.h"

#include "gamestateio/loading/EntityResource.h"
#include "resources/TextureStorage.h"

#include "gameplay/StatusEffect.h"

#include "gameplay/handlers/StatusEffectHandler.h"
#include "graphics/GifDrawer.h"
#include "ui/Window.h"
#include "ui/player/elements/InventorySlot.h"

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
#include "world/WorldManager.h"

#include "gamestateio/GameLoader.h"
#include "gamestateio/GameSaver.h"

#include "ui/PlayerUI.h"

#include "ui/menus/GameMenu.h"
#include "ui/menus/MainMenu.h"

#include "ui/UIManager.h"

#include "Game.h"

#endif  //MAGEQUEST_SRC_INCLUDES_H_
