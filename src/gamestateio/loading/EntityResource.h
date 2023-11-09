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
  void LoadSpriteSheet(const std::string& name) {

    std::string path = ASSET_PATH + "projectiles/" += name + std::to_string(0) + ".png";
    frames.push_back(LoadTexture(path.c_str()));
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
