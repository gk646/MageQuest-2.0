#ifndef MAGEQUEST_SRC_WORLD_SOUNDMANAGER_H_
#define MAGEQUEST_SRC_WORLD_SOUNDMANAGER_H_


namespace SoundManager {

inline static void Update() noexcept {
  MusicStreamer::Update();
  if (CURRENT_ZONE == Zone::Oasis) {
    MusicStreamer::StartPlaylist(&sound::music::dungeonAmbience);
  }
}
}  // namespace SoundManager
#endif  //MAGEQUEST_SRC_WORLD_SOUNDMANAGER_H_
