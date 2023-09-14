#ifndef MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_



struct QuestPanel final : public Window {
  static constexpr int width = 500;
  QuestPanel() noexcept
      : Window(SCREEN_WIDTH * 0.1F, SCREEN_HEIGHT * 0.2F, width, 500, 25, "Character",
               KEY_J) {

  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
