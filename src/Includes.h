#ifndef MAGEQUEST_SRC_INCLUDES_H_
#define MAGEQUEST_SRC_INCLUDES_H_

//stdlib
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
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

//cxstructs
#include "cxstructs/Geometry.h"
#include "cxstructs/vec.h"
#include "cxutil/cxtime.h."

//Mage Quest
#include "system/Colors.h"
#include "system/Definitions.h"
#include "system/Enums.h"
#include "system/Util.h"

#include "gameplay/Stats.h"
#include "graphics/maps/Map.h"

#include "gameplay/StatusEffect.h"

#include "system/GlobalVariables.h"

#include "gameplay/StatusEffectHandler.h"

#include "gamestateio/GameLoader.h"
#include "graphics/WorldRender.h"
#include "system/BenchMark.h"
#include "system/Util.h"
#include "ui/UIManager.h"

#include "Game.h"

#endif  //MAGEQUEST_SRC_INCLUDES_H_
