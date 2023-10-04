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
  Zone zone;
  int16_t** map_back_ground;
  int16_t** map_middle_ground;
  int16_t** map_fore_ground;
  bool** map_cover;
  std::vector<SpawnTrigger>* spawnTriggers;

  int16_t map_size;

  Map(const std::string& name, int16_t size, Zone zone) noexcept
      : map_size(size), zone(zone) {
    map_back_ground = LoadMapData(name + "/" + name + "_BG");
    map_middle_ground = LoadMapData(name + "/" + name + "_BG1");
    map_fore_ground = LoadMapData(name + "/" + name + "_FG");
    map_cover = LoadMapCover();
    spawnTriggers = LoadSpawnTriggers(name + "/" + name);
  }
  ~Map() {
    delete_2D_array(map_back_ground, map_size);
    delete_2D_array(map_middle_ground, map_size);
    delete_2D_array(map_fore_ground, map_size);
    delete spawnTriggers;
  }

  [[nodiscard]] int16_t** LoadMapData(const std::string& name) const noexcept {
    std::string filepath = ASSET_PATH + "Maps/" + name + ".csv";
    std::ifstream infile(filepath);

    if (!infile.is_open()) {
      return nullptr;
    }

    auto arr = get_2D_array<int16_t>(map_size, map_size);
    std::string line;
    for (int i = 0; i < map_size; ++i) {
      if (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;
        for (int b = 0; b < map_size; ++b) {
          if (std::getline(iss, token, ',')) {
            arr[b][i] = static_cast<int16_t>(std::stoi(token));
          }
        }
      }
    }

    infile.close();
    return arr;
  }
  [[nodiscard]] bool** LoadMapCover() const noexcept {
    auto arr = get_2D_array<bool>(map_size, map_size);
    for (uint_fast32_t i = 0; i < map_size; i++) {
      for (uint_fast32_t j = 0; j < map_size; j++) {
        arr[i][j] = true;
      }
    }
    return arr;
  }

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
    int y = 0;
    bool isSingular = true;
    int amount = 0;
    int level = 0;
    TriggerSpreadType spread_type = TriggerSpreadType::MIXED_GOBLIN;
    MonsterType type = MonsterType::ANY;
    std::vector<std::string> parts;
    while (std::getline(file, line)) {
      if (line.contains("\"height\"")) {
        parts = split(line, ':');
        height = std::stoi(parts[1]);
      } else if (line.contains("\"name\"")) {
        parts = split(line, ':');
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
        parts = split(line, ':');
        width = std::stoi(parts[1]);
      } else if (line.contains("\"x\"")) {
        parts = split(line, ':');
        x = std::stoi(parts[1]);
      } else if (line.contains("\"y\"")) {
        parts = split(line, ':');
        y = std::stoi(parts[1]);
        isSingular = amount == 0;
        vector->emplace_back(PointI{x * 3, y * 3}, PointI{width * 3, height * 3}, false,
                             isSingular, spread_type, level, type, amount);
        RESET_TO_DEFAULTS()
      }
    }

    return vector;
  }

 private:
  inline static bool ParseTriggerType(const std::string& s, TriggerSpreadType& ts,
                                      MonsterType& mt) noexcept {
    auto it = stringToMonsterID.find(s);
    if (it != stringToMonsterID.end()) {
      mt = it->second;
      return true;
    }
    return false;
  }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
