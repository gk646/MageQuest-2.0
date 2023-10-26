#ifndef MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_
#define MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_

struct Track {
  Music* music;
  float volume;
  bool markedRemoval = false;
  [[nodiscard]] inline bool Update() noexcept {
    PlayMusicStream(*music);
    UpdateMusicStream(*music);
    if (markedRemoval) {
      volume -= 0.01F;
      SetMusicVolume(*music, volume);
    }
    return markedRemoval && volume == 0;
  }
};
namespace MusicStreamer {
inline static std::vector<Track> currentTracks;
inline void Update() noexcept {
  for (auto it = currentTracks.begin(); it != currentTracks.end();) {
    if (it->Update()) {
      it = currentTracks.erase(it);
    } else {
      it++;
    }
  }
}
inline void RemoveTrack(Music* music, bool fadeOut = true) noexcept {
  for (auto t : currentTracks) {
    if (t.music == music) {
      t.markedRemoval = true;
      if (!fadeOut) {
        t.volume = 0;
      }
    }
  }
}
inline void AddTrack(Music* music, bool fadeIn = true) noexcept {
  currentTracks.emplace_back(Track{music, fadeIn ? 1.0F : 1.0F});
}
}  // namespace MusicStreamer
#endif  //MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_
