#ifndef MAGE_QUEST_SRC_GRAPHICS_TILEMANAGER_H_
#define MAGE_QUEST_SRC_GRAPHICS_TILEMANAGER_H_

namespace TileLoader {
inline static std::vector<Image> images;
//loads the textures from the images into the GPU (no IO)
static void LoadToGPU() {
  for (uint_fast32_t i = 0; i < TEXTURE_COUNT; i++) {
    TILES[i] = LoadTextureFromImage(images[i]).id;
    UnloadImage(images[i]);
  }
  images.clear();
  images.shrink_to_fit();
}

//Load the Textures as images on the cpu
static void LoadTextureSpan(int start, int end) {
  images.reserve((end - start) + 10);
  for (uint_fast32_t i = start; i < end; ++i) {
    sprintf(TEXT_BUFFER, "%stextures/Tiles/%d.png", ASSET_PATH.c_str(), i);
    images.emplace_back(LoadImageR(TEXT_BUFFER));
  }
}

//Load tile collision from the .tsx file
static void LoadTileCollisionFromFile(const std::string& path) {
  std::ifstream file(ASSET_PATH + path);

  if (!file.is_open()) {
    std::cerr << "Failed to open" << path << std::endl;
  }

  std::string line;
  for (uint_fast32_t i = 0; i < 8; i++) {
    std::getline(file, line);
  }

  //<tile id="92" type="1"/>
  while (std::getline(file, line)) {
    std::size_t start = line.find("id=\"");
    if (start != std::string::npos) {
      start += 4;
      std::size_t end = line.find('\"', start);
      if (end != std::string::npos) {
        std::string id_str = line.substr(start, end - start);
        int id = std::stoi(id_str);
        COLLISIONS[id] = C_SOLID;
      }
    }
  }
}
static void Load() {
  LoadTextureSpan(0, TEXTURE_COUNT);
  LoadTileCollisionFromFile("textures/spriteSheet.tsx");
}
}  // namespace TileLoader

#endif  //MAGE_QUEST_SRC_GRAPHICS_TILEMANAGER_H_
