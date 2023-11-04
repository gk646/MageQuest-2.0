#ifndef MAGEQUEST_SRC_UI_ELEMENTS_SOUNDCOMPONENT_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_SOUNDCOMPONENT_H_

//A component to play overlapping sounds // automatically cleans up
struct SoundComponent {
  Sound alias[2]{};
  uint8_t count = 0;
  inline void PlaySound(const Sound* sound) noexcept {
    UnloadSoundAlias(alias[count]);
    alias[count] = LoadSoundAlias(*sound);
    PlaySoundR(alias[count]);
    count = (count + 1) % 2;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_SOUNDCOMPONENT_H_
