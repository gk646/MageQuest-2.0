#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLPANEL_H_

struct SkillPanel final : Window {
  inline static constexpr float WINDOW_WIDTH = 600;
  inline static constexpr float WINDOW_HEIGHT = 500;
  inline static char name[] = "Skills";
  SkillPanel()
      : Window((SCREEN_WIDTH - WINDOW_WIDTH) / 2, (SCREEN_HEIGHT - WINDOW_HEIGHT) / 2,
               WINDOW_WIDTH, WINDOW_HEIGHT, 20, name, KEY_P, sound::EMPTY_SOUND,
               sound::EMPTY_SOUND) {}
  void Draw() noexcept {
    WINDOW_LOGIC()
    DrawWindow();
  }
  void Update() noexcept { WINDOW_UPDATE(); }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLPANEL_H_
