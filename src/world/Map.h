#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
#include "elements/SpawnTrigger.h"

#define RESET_TO_DEFAULTS()                      \
  height = 0;                                    \
  width = 0;                                     \
  x = 0;                                         \
  y = 0;                                         \
  isSingular = true;                             \
  amount = 0;                                    \
  level = 0;                                     \
  spread_type = TriggerSpreadType::MIXED_GOBLIN; \
  type = MonsterType::ANY;

struct Map {
  int16_t** mapBackGround;
  int16_t** mapMiddleGround;
  int16_t** mapForeGround;
  bool** mapCover;
  std::vector<SpawnTrigger>* spawnTriggers;
  //All maps are squares
  int16_t mapSize;
  Zone zone;

  Map(const std::string& name, int16_t size, Zone zone) noexcept
      : mapSize(size), zone(zone) {
    mapBackGround = LoadMapData(name + "/" + name + "_BG");
    mapMiddleGround = LoadMapData(name + "/" + name + "_BG1");
    mapForeGround = LoadMapData(name + "/" + name + "_FG");
    mapCover = Util::Create2DArr<bool>(mapSize, mapSize);
    spawnTriggers = LoadSpawnTriggers(name + "/" + name);
  }
  ~Map() {
    Util::Delete2DArr(mapBackGround, mapSize);
    Util::Delete2DArr(mapMiddleGround, mapSize);
    Util::Delete2DArr(mapForeGround, mapSize);
    delete spawnTriggers;
  }
  //Parses the ".csv" map files
  [[nodiscard]] int16_t** LoadMapData(const std::string& name) const noexcept {
    std::string filepath = ASSET_PATH + "Maps/" + name + ".csv";
    std::ifstream infile(filepath);
    if (!infile.is_open()) {
      return nullptr;
    }

    auto arr = Util::Create2DArr<int16_t>(mapSize, mapSize);
    std::string line;
    line.reserve(200);

    char* endPtr;
    for (int i = 0; i < mapSize; ++i) {
      if (std::getline(infile, line)) {
        const char* start = line.c_str();
        for (int b = 0; b < mapSize; ++b) {
          auto value = static_cast<int16_t>(std::strtol(start, &endPtr, 10));
          if (worldObjectTable.contains(value)) {
            RegisterInteractableObject(WorldObjectType(value), {b, i}, zone);
            value = -1;
          }
          arr[b][i] = value;
          start = endPtr + 1;
        }
      }
    }
    return arr;
  }

  //Parses the maps ".tmj" file to load the spawn triggers
  [[nodiscard]] static std::vector<SpawnTrigger>* LoadSpawnTriggers(
      const std::string& name) noexcept {
    auto vector = new std::vector<SpawnTrigger>;
    std::string filepath = ASSET_PATH + "Maps/" + name + ".tmj";
    std::ifstream file(filepath);

    if (!file.is_open()) {
      return nullptr;
    }
    std::string line;
    bool insideObjectLayer = false;
    while (std::getline(file, line)) {
      if (line.contains("\"objects")) {
        insideObjectLayer = true;
        break;
      }
    }

    if (!insideObjectLayer) return nullptr;
    int height = 0;
    int width = 0;
    int x = 0;
    int y;
    bool isSingular = true;
    int amount = 0;
    int level = 0;
    TriggerSpreadType spread_type = TriggerSpreadType::DEFAULT;
    MonsterType type = MonsterType::ANY;
    std::vector<std::string> parts;
    while (std::getline(file, line)) {
      if(line.contains("opacity")) return vector;
      if (line.contains("\"height\"")) {
        parts = Util::SplitString(line, ':');
        height = std::stoi(parts[1]);
      } else if (line.contains("\"name\"")) {
        parts = Util::SplitString(line, ':');
        std::string triggerName = parts[1].substr(1, parts[1].find_last_of('"'));

        size_t numStart = triggerName.find_first_of("0123456789");
        if (numStart != std::string::npos) {
          std::string namePart = triggerName.substr(0, numStart);
          std::string numberPart = triggerName.substr(numStart);
          int num = std::stoi(numberPart);
          if (ParseTriggerType(namePart, spread_type, type)) {
            level = num;
          } else {
            amount = num;
          }
        }
      } else if (line.contains("\"width\"")) {
        parts = Util::SplitString(line, ':');
        width = std::stoi(parts[1]);
      } else if (line.contains("\"x\"")) {
        parts = Util::SplitString(line, ':');
        x = std::stoi(parts[1]);
      } else if (line.contains("\"y\"")) {
        parts = Util::SplitString(line, ':');
        y = std::stoi(parts[1]);
        isSingular = amount == 0;
        if (type != MonsterType::ANY || spread_type != TriggerSpreadType::DEFAULT) {
          vector->emplace_back(PointI{x * 3, y * 3}, PointI{width * 3, height * 3}, false,
                               isSingular, spread_type, level, type, amount);
        }
        RESET_TO_DEFAULTS()
      }
    }

    return vector;
  }

 private:
  static void RegisterInteractableObject(WorldObjectType type,
                                         const cxstructs::PointI& pos, Zone zone);
  inline static bool ParseTriggerType(const std::string& s, TriggerSpreadType& ts,
                                      MonsterType& mt) noexcept {
    auto it = stringToMonsterID.find(s);
    if (it != stringToMonsterID.end()) {
      mt = it->second;
      return true;
    } else {
      auto triggerIt = stringToTriggerType.find(s);
      if (triggerIt != stringToTriggerType.end()) {
        ts = triggerIt->second;
      }
      return false;
    }
  }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
