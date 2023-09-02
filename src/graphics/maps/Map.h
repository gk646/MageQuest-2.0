#ifndef MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
#define MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_




struct Map {
  int16_t** map_back_ground;
  int16_t** map_middle_ground;
  int16_t** map_fore_ground;

  int16_t map_size;

  Map(std::string name, int size) : map_size(size) {
    map_back_ground = load_map_data(name + "/" + name + "_BG");
    map_middle_ground = load_map_data(name + "/" + name + "_BG1");
    map_fore_ground = load_map_data(name + "/" + name + "_FG");
  }
  ~Map() {
    delete_2D_array(map_back_ground, map_size);
    delete_2D_array(map_middle_ground, map_size);
    delete_2D_array(map_fore_ground, map_size);
  }

  [[nodiscard]] int16_t** load_map_data(const std::string& name) const {
    std::string filepath = "res/Maps/" + name + ".csv";
    std::ifstream infile(filepath);

    if (!infile.is_open()) {
      throw std::runtime_error("Could not open file");
    }

    auto arr = get_2D_array<int16_t>(map_size, map_size);
    std::string line;
    for (int i = 0; i < map_size; ++i) {
      if (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string token;
        for (int b = 0; b < map_size; ++b) {
          if (std::getline(iss, token, ',')) {
            arr[b][i] = std::stoi(token);
          }
        }
      }
    }

    infile.close();
    return arr;
  }
};
#endif  //MAGE_QUEST_SRC_GRAPHICS_MAPS_MAP_H_
