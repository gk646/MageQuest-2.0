#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCE_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCE_H_

inline static void LoadImagesIntoVector(const std::string& name,
                                        std::vector<Texture>& vec) noexcept {
  for (uint_fast32_t j = 0; j < 50; ++j) {
    std::string filePath = ASSET_PATH + name + std::to_string(j) + ".png";
    if (std::filesystem::exists(filePath)) {
      vec.push_back(LoadTexture(filePath.c_str()));
    } else {
      break;
    }
  }
}

inline static void LoadSoundIntoVector(const std::string& name, std::vector<Sound>& vec,
                                       float volume = 1) noexcept {
  for (uint_fast32_t j = 0; j < 50; ++j) {
    std::string filePath = ASSET_PATH + name + std::to_string(j) + ".wav";
    if (std::filesystem::exists(filePath)) {
      auto sound = LoadSound(filePath.c_str());
      SetSoundVolume(sound, volume);
      vec.emplace_back(sound);
    } else {
      break;
    }
  }
}
// Helper function to load a single frame from a sprite sheet with offset for centering
Texture LoadFrame(const Image& spriteSheet, int frameWidth, int frameHeight, int x, int y,
                  int xOffset = 0, int yOffset = 0) {
  int finalWidth = frameWidth + 2 * xOffset;
  int finalHeight = frameHeight + 2 * yOffset;

  RectangleR sourceRect = {
      static_cast<float>(x * frameWidth), static_cast<float>(y * frameHeight),
      static_cast<float>(frameWidth), static_cast<float>(frameHeight)};

  RectangleR destRect = {static_cast<float>(xOffset), static_cast<float>(yOffset),
                         static_cast<float>(frameWidth), static_cast<float>(frameHeight)};

  Image frame =
      GenImageColor(finalWidth, finalHeight, BLANK);  // Use BLANK for transparency

  ImageDraw(&frame, spriteSheet, sourceRect, destRect, WHITE);

  Texture texture = LoadTextureFromImage(frame);

  UnloadImage(frame);

  return texture;
}
//Method to load all frames from a sprite sheet
void LoadSpriteSheetFrames(const std::string& name, int frameWidth, int frameHeight,
                           std::vector<Texture>& frames, int xOffset, int yOffset) {
  std::string path = ASSET_PATH + "projectiles/" + name + ".png";
  Image spriteSheet = LoadImageR(path.c_str());

  int framesHorizontal = spriteSheet.width / frameWidth;
  int framesVertical = spriteSheet.height / frameHeight;

  for (int y = 0; y < framesVertical; ++y) {
    for (int x = 0; x < framesHorizontal; ++x) {
      Texture frame =
          LoadFrame(spriteSheet, frameWidth, frameHeight, x, y, xOffset, yOffset);
      frames.push_back(frame);
    }
  }
  UnloadImage(spriteSheet);
}
struct MonsterResource {
  static constexpr int MAX_LOAD_NUM = 20;
  std::vector<Texture> attack1{};
  std::vector<Texture> attack2{};
  std::vector<Texture> attack3{};
  std::vector<Texture> attack4{};
  std::vector<Texture> attack5{};
  std::vector<Texture> special{};

  std::vector<Texture> idle{};
  std::vector<Texture> idle2{};
  std::vector<Texture> walk{};

  std::vector<Texture> death{};

  std::vector<Sound> attackSounds{};
  void Load(const std::string& name) {
    load_textures(name);
    load_sound(name);
  }

 private:
  void load_textures(const std::string& name) {
    const std::array<std::string, 12> folder_names = {
        "attack1/", "attack2/", "attack3/", "attack4/", "attack5/",
        "special/", "idle/",    "idle2/",   "walk/",    "death/"};

    std::array<std::vector<Texture>*, 12> texture_arrays = {
        &attack1, &attack2, &attack3, &attack4, &attack5,
        &special, &idle,    &idle2,   &walk,    &death};

    std::string path;
    for (size_t i = 0; i < folder_names.size(); ++i) {
      LoadImagesIntoVector("Entities/" + name + folder_names[i], *texture_arrays[i]);
    }
  }
  void load_sound(const std::string& name) {
    std::string path;
    std::string soundPath = name;
    size_t pos = soundPath.find("enemies/");

    if (pos != std::string::npos) {
      soundPath.erase(pos, std::string("enemies/").length());
    }

    for (uint_fast32_t i = 0; i < MAX_LOAD_NUM; i++) {
      path = ASSET_PATH + "sound/effects/entities/" +=
          soundPath + std::to_string(i) + ".wav";
      if (std::filesystem::exists(path)) {
        auto sound = LoadSound(path.c_str());
        SetSoundVolume(sound, 0.5F);
        attackSounds.emplace_back(sound);
      } else {
        break;
      }
    }
  }
};

struct ProjectileResources {
  std::vector<Texture> frames{};

  void Load(const std::string& name) { load_textures(name); }
  void LoadSpriteSheet(const std::string& name, int frameWidth, int frameHeight,
                       int offSetX = 0, int offSetY = 0) {
    LoadSpriteSheetFrames(name, frameWidth, frameHeight, frames, offSetX, offSetY);
  }

 private:
  void load_textures(const std::string& name) {
    std::string path;
    for (uint_fast32_t j = 0; j < 30; ++j) {
      path = ASSET_PATH + "projectiles/" += name + std::to_string(j) + ".png";
      if (std::filesystem::exists(path)) {
        frames.push_back(LoadTexture(path.c_str()));
      } else {
        break;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCE_H_
