#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
struct SoundLoader{

  static void load(){
    sound::intro = LoadSound((ASSET_PATH+"sound/music/intro.wav").c_str());
  }
};
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_SOUNDLOADER_H_
