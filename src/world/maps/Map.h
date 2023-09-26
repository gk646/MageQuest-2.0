#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_

struct Map {
  Zone zone;
  int16_t** map_back_ground;
  int16_t** map_middle_ground;
  int16_t** map_fore_ground;
  bool** map_cover;

  int16_t map_size;

  Map(const std::string& name, int size, Zone zone) noexcept
      : map_size(size), zone(zone) {
    map_back_ground = load_map_data(name + "/" + name + "_BG");
    map_middle_ground = load_map_data(name + "/" + name + "_BG1");
    map_fore_ground = load_map_data(name + "/" + name + "_FG");
    map_cover = load_map_cover("");
  }
  ~Map() {
    delete_2D_array(map_back_ground, map_size);
    delete_2D_array(map_middle_ground, map_size);
    delete_2D_array(map_fore_ground, map_size);
  }

  [[nodiscard]] int16_t** load_map_data(const std::string& name) const noexcept {
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
  [[nodiscard]] bool** load_map_cover(const std::string& name) const noexcept {
    auto arr = get_2D_array<bool>(map_size, map_size);
    for (uint_fast32_t i = 0; i < map_size; i++) {
      for (uint_fast32_t j = 0; j < map_size; j++) {
        arr[i][j] = true;
      }
    }
    return arr;
  }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
