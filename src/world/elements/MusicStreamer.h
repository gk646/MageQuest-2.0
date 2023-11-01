#ifndef MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_
#define MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_

struct Playlist {
  std::vector<Music> tracks;
  int currentTrack = 0;
  bool isPlaying = false;
  //Loads all ".mp3" files from "0.mp3" ascending inside the given folder
  void Load(const std::string& folderName) {
    std::string path;
    for (uint_fast32_t i = 0; i < 20; i++) {
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
struct Track {
  Music* music;
  float volume;
  bool markedRemoval = false;
  [[nodiscard]] inline bool Update() noexcept {
    UpdateMusicStream(*music);
    if (markedRemoval) {
      volume -= 0.005F;
      SetMusicVolume(*music, volume);
      if (volume <= 0) {
        StopMusicStream(*music);
      }
    } else if (volume < 1.0F) {
      volume += 0.005F;
      SetMusicVolume(*music, volume);
    } else {
      markedRemoval = GetMusicTimeLength(*music) - GetMusicTimePlayed(*music) < 3.3F;
    }
    return markedRemoval && volume <= 0;
  }
};

namespace MusicStreamer {
inline static std::vector<Track> currentTracks;
inline static std::vector<Playlist*> playingPlaylists;
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
  PlayMusicStream(*music);
  currentTracks.emplace_back(Track{music, fadeIn ? 0.0F : 1.0F});
}
//Adds the playlist to "playingPlaylists" and plays the next track / updates itself automatically
inline void StartPlaylist(Playlist* playlist) noexcept {
  if (!playlist || playlist->isPlaying) return;
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
  }
}
}  // namespace MusicStreamer

void Playlist::PlayNextTrack() noexcept {
  currentTrack = (currentTrack + 1) % (int)(tracks.size());
  MusicStreamer::AddTrack(&tracks[currentTrack]);
}
void Playlist::Update() noexcept {
  if (!IsMusicStreamPlaying(tracks[currentTrack])) {
    PlayNextTrack();
  }
}
#endif  //MAGEQUEST_SRC_WORLD_MUSICSTREAMER_H_
