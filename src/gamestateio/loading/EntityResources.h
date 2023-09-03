#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCES_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCES_H_


struct MonsterResources{
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
};


struct ProjectileResources{

};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_ENTITYRESOURCES_H_
