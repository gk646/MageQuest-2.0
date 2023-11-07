#ifndef MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MUSICLOADER_H_
#define MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MUSICLOADER_H_
namespace MusicLoader {

static void Load() noexcept {
  sound::music::bossMusic.Load("bossMusic/");
  sound::music::dungeonAmbience.Load("dungeonAmbience/");
  sound::music::forestAmbience.Load("forestAmbience/");
  sound::music::forestNightAmbience.Load("forestNightAmbience/");
  sound::music::waterAmbience.Load("waterAmbience/");
  sound::music::caveAmbience.Load("caveAmbience/");

  //TODO add town ambience
  //LoadPlayList("townAmbience/", sound::music::townAmbience);

  sound::music::hillcrestVillage =
      LoadMusicStream((ASSET_PATH + "sound/music/hillcrest.mp3").c_str());
  sound::music::firePlace =
      LoadMusicStream((ASSET_PATH + "sound/music/fireplace.mp3").c_str());
  sound::music::lava = LoadMusicStream((ASSET_PATH + "sound/music/lava.mp3").c_str());
}

}  // namespace MusicLoader
#endif  //MAGEQUEST_SRC_GAMESTATEIO_LOADING_LOADERS_MUSICLOADER_H_
