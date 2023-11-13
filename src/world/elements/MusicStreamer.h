#ifndef MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_
#define MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_

//A wrapper for a list of "Music"instances / holds all the instances directly
//Progressed automatically by calling the "Update" method each tick
struct Playlist {
  std::vector<Music> tracks;
  int currentTrack = 0;
  bool isPlaying = false;
  //Loads all ".mp3" files from "0.mp3" ascending inside the given folder
  void Load(const std::string& folderName) {
    std::string path;
    //To prevent indefinite loop
    for (uint_fast32_t i = 0; i < 50; i++) {
      path = ASSET_PATH + "sound/music/" += folderName + std::to_string(i) + ".mp3";
      if (std::filesystem::exists(path)) {
        tracks.push_back(LoadMusicStream(path.c_str()));
      } else {
        break;
      }
    }
  }
  inline void Update() noexcept;
  inline void PlayNextTrack() noexcept;
};
//A wrapper for a "Music" ptr with volume control and dynamic removal
struct Track {
  static constexpr float kVolumeFadeStep = 0.005F;
  static constexpr float kEndTrackThreshold = 3.3F;
  Music* music = nullptr;
  float volume;
  bool markedForRemoval = false;
  [[nodiscard]] inline bool Update() noexcept {
    UpdateMusicStream(*music);
    if (markedForRemoval) {
      volume -= kVolumeFadeStep;
      SetMusicVolume(*music, volume);
      if (volume <= 0.15F) {
        StopMusicStream(*music);
      }
    } else if (volume < 1.0F) {
      volume += kVolumeFadeStep;
      SetMusicVolume(*music, volume);
    } else {
      markedForRemoval =
          GetMusicTimeLength(*music) - GetMusicTimePlayed(*music) < kEndTrackThreshold;
    }
    return markedForRemoval && volume <= 0;
  }
};

//Automatic music stream // never deletes the given base resources
namespace MusicStreamer {
inline static constexpr uint_fast32_t kMaxTracks = 50;
inline static std::vector<Track> currentTracks;
inline static std::vector<Playlist*> playingPlaylists;
//Progresses both sounds and playlists and handles deletion of finished sounds
inline void Update() noexcept {
  for (auto it = currentTracks.begin(); it != currentTracks.end();) {
    if (it->Update()) {
      it = currentTracks.erase(it);
    } else {
      it++;
    }
  }
  for (auto& p : playingPlaylists) {
    p->Update();
  }
}
//Removes the first track with the given music
inline void RemoveTrack(Music* music, bool fadeOut = true) noexcept {
  for (auto& t : currentTracks) {
    if (t.music == music) {
      t.markedForRemoval = true;
      if (!fadeOut) {
        t.volume = 0;
      }
      return;
    }
  }
}
//Adds the music as a track // Doesn't allow duplicates
inline void AddTrack(Music* music, bool fadeIn = true) noexcept {
  if (currentTracks.size() == kMaxTracks) return;
  for (const auto& track : currentTracks) {
    if (track.music == music) return;
  }
  PlayMusicStream(*music);
  currentTracks.emplace_back(Track{music, fadeIn ? 0.15F : 1.0F});
}
//Adds the playlist to "playingPlaylists" and plays the next track / updates itself automatically
inline void StartPlaylist(Playlist* playlist) noexcept {
  if (!playlist || playlist->isPlaying || playlist->tracks.empty()) return;
  playlist->isPlaying = true;
  playlist->PlayNextTrack();
  playingPlaylists.push_back(playlist);
}
//Removes the playlist from "playingPlaylists" / cleans up its sounds
inline void StopPlaylist(Playlist* playlist) noexcept {
  if (!playlist || !playlist->isPlaying) return;
  for (auto it = playingPlaylists.begin(); it != playingPlaylists.end();) {
    if ((*it) == playlist) {
      (*it)->isPlaying = false;
      RemoveTrack(&(*it)->tracks[(*it)->currentTrack]);
      playingPlaylists.erase(it);
      return;
    }
    ++it;
  }
}
}  // namespace MusicStreamer

void Playlist::PlayNextTrack() noexcept {
  if (tracks.empty()) return;
  currentTrack = (currentTrack + 1) % (int)(tracks.size());
  MusicStreamer::AddTrack(&tracks[currentTrack]);
}
void Playlist::Update() noexcept {
  if (isPlaying && !IsMusicStreamPlaying(tracks[currentTrack])) {
    PlayNextTrack();
  }
}
#endif  //MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_
