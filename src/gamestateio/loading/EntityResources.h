#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCES_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCES_H_

struct MonsterResources {
  std::vector<Texture> attack1{};
  std::vector<Texture> attack2{};
  std::vector<Texture> attack3{};
  std::vector<Texture> attack4{};
  std::vector<Texture> attack5{};
  std::vector<Texture> special{};

  std::vector<Texture> idle{};
  std::vector<Texture> idle2{};
  std::vector<Texture> walk{};
  std::vector<Texture> run{};
  std::vector<Texture> run_mirrored{};

  std::vector<Texture> death{};

  std::vector<Sound> attack_sound{};
  static constexpr int load_num = 20;
  void load(const std::string& name) {
    load_textures(name);
    load_sound(name);
  }

  void load_textures(const std::string& name) {
    const std::array<std::string, 14> folder_names = {
        "attack1/",
        "attack2/",
        "attack3/",
        "attack4/",
        "attack5/",
        "special/",
        "idle/",
        "idle2/",
        "walk/",
        "run/",
        "runMirror/",
        "death/"
    };

    std::array<std::vector<Texture>*, 14> texture_arrays = {
        &attack1,
        &attack2,
        &attack3,
        &attack4,
        &attack5,
        &special,
        &idle,
        &idle2,
        &walk,
        &run,
        &run_mirrored,
        &death
    };

    std::string path;
    for (size_t i = 0; i < folder_names.size(); ++i) {
      for (uint_fast32_t j = 0; j < load_num; ++j) {
        path = ASSET_PATH + "Entities/" + name + folder_names[i] + std::to_string(j) + ".png";
        if (std::filesystem::exists(path)) {
          texture_arrays[i]->push_back(LoadTexture(path.c_str()));
        } else {
          break;
        }
      }
    }
  }
  void load_sound(const std::string& name) {

  }
};

struct ProjectileResources {};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCES_H_
