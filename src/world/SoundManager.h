#ifndef MAGEQUEST_SRC_WORLD_SOUNDMANAGER_H_
#define MAGEQUEST_SRC_WORLD_SOUNDMANAGER_H_

namespace SoundManager {
inline static void UpdateZoneBasedAudio() noexcept {
  //Start forest playlist
  if (CURRENT_ZONE == Zone::Oasis || CURRENT_ZONE == Zone::Tutorial ||
      CURRENT_ZONE == Zone::The_Grove || CURRENT_ZONE == Zone::Hillcrest ||
      CURRENT_ZONE == Zone::GrassLands) {
    MusicStreamer::StartPlaylist(&sound::music::forestAmbience);
  } else {
    MusicStreamer::StopPlaylist(&sound::music::forestAmbience);
  }

  //Start dungeon playlist
  if (CURRENT_ZONE == Zone::Ruin_Dungeon || CURRENT_ZONE == Zone::Goblin_Cave ||
      CURRENT_ZONE == Zone::Hillcrest_Hermit_Cave) {
    MusicStreamer::StartPlaylist(&sound::music::dungeonAmbience);
  } else {
    MusicStreamer::StopPlaylist(&sound::music::dungeonAmbience);
  }

  //Start cave ambience
  if (CURRENT_ZONE == Zone::Oasis_Cave || CURRENT_ZONE == Zone::Hillcrest_Mountain_Cave) {
    MusicStreamer::StartPlaylist(&sound::music::caveAmbience);
  } else {
    MusicStreamer::StopPlaylist(&sound::music::caveAmbience);
  }
}
inline static void Update() noexcept {
  MusicStreamer::Update();
  UpdateZoneBasedAudio();
}
}  // namespace SoundManager
#endif  //MAGEQUEST_SRC_WORLD_SOUNDMANAGER_H_
