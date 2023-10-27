#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MUSICLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MUSICLOADER_H_
namespace MusicLoader {
static void LoadPlayList(const std::string& name, std::vector<Music>& playList) {
  std::string path;

  for (uint_fast32_t i = 0; i < 20; i++) {
    path = ASSET_PATH + "sound/music/" += name + std::to_string(i) + ".mp3";
    if (std::filesystem::exists(path)) {
      playList.push_back(LoadMusicStream(path.c_str()));
    } else {
      break;
    }
  }
}
static void Load() noexcept {
  LoadPlayList("bossMusic/", sound::music::bossMusic);
  LoadPlayList("dungeonAmbience/", sound::music::dungeonAmbience);
  LoadPlayList("forestAmbience/", sound::music::forestAmbience);
  //TODO add town ambience
  //LoadPlayList("townAmbience/", sound::music::townAmbience);
  LoadPlayList("waterAmbience/", sound::music::waterAmbience);

  sound::music::hillcrestVillage = LoadMusicStream((ASSET_PATH+"sound/music/hillcrest.mp3").c_str());
  sound::music::firePlace = LoadMusicStream((ASSET_PATH+"sound/music/fireplace.mp3").c_str());
  sound::music::lava = LoadMusicStream((ASSET_PATH+"sound/music/lava.mp3").c_str());
  sound::music::moonLightSonata = LoadMusicStream((ASSET_PATH+"sound/music/moonlight_sonata.mp3").c_str());
}

}  // namespace MusicLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MUSICLOADER_H_