#ifndef MAGEQUEST_SRC_INCLUDES_H_
#define MAGEQUEST_SRC_INCLUDES_H_

//stdlib
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <shared_mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>
#include <filesystem>

//raylib
#define RAYLIB_IMPLEMENTATION
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//SFML
#include <SFML/Network.hpp>

//cxstructs
#include "cxstructs/Geometry.h"
#include "cxstructs/vec.h"
#include "cxutil/cxtime.h."

//Mage Quest
#include "system/Colors.h"
#include "system/Definitions.h"
#include "system/Enums.h"
#include "system/Util.h"
#include "ui/StyleSheet.h"

#include "gameplay/Stats.h"
#include "graphics/maps/Map.h"

#include "gameplay/StatusEffect.h"

#include "gameplay/StatusEffectHandler.h"

#include "system/GameSettings.h"
#include "system/GlobalVariables.h"

//after global variables
#include "gamestateio/loading/EntityResources.h"
#include "graphics/WorldManager.h"
#include "graphics/WorldRender.h"
#include "system/BenchMark.h"
#include "system/SoundStorage.h"
#include "system/TextureStorage.h"
#include "ui/game/LoadingScreen.h"
#include "ui/menus/ServerBrowser.h"
#include "ui/menus/SettingsMenu.h"
#include "ui/player/PlayerUI.h"

#include "gamestateio/loading/LoadingUtil.h"

#include "gamestateio/GameLoader.h"

#include "ui/menus/GameMenu.h"
#include "ui/menus/MainMenu.h"

#include "ui/UIManager.h"

#include "Game.h"

#endif  //MAGEQUEST_SRC_INCLUDES_H_
