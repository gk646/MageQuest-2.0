#ifndef MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_

#include "../elements/ExpandableMenu.h"

struct QuestSidePanel final : public ExpandablePanel {
  static inline char HEADER[] = "Objectives";
  QuestSidePanel() : ExpandablePanel(270, 150, 16, MINECRAFT_BOLD, HEADER) {}
  void DrawContent() noexcept final {
    if (PLAYER_QUESTS.HasActiveQuest()) {
      Util::DrawOutlineText(MINECRAFT_BOLD, SCALE(16),
                            PLAYER_QUESTS.activeQuest->name.c_str(), body.x + SCALE(2),
                            body.y + SCALE(35), 1, Colors::LightGrey, Colors::black);
      Util::DrawOutlineText(MINECRAFT_REGULAR, SCALE(15),
                            PLAYER_QUESTS.activeQuest->GetActiveObjective().c_str(),
                            body.x + SCALE(20), body.y + SCALE(50), 1, Colors::LightGrey,
                            Colors::black);
    } else {
      Util::DrawOutlineText(MINECRAFT_REGULAR, SCALE(15), "No active Quest",
                            body.x + SCALE(20), body.y + SCALE(50), 1, Colors::LightGrey,
                            Colors::black);
    }
  }
};

struct QuestPanel final : public Window {
  QuestSidePanel side_panel;
  ScrollPane questMenu{
      {(SCREEN_WIDTH - WIDTH) / 2, (SCREEN_HEIGHT - HEIGHT) / 2, WIDTH / 2, HEIGHT - 23},
      new ExpandableQuestMenu(WIDTH / 2, HEIGHT)};
  static inline char HEADER[] = "Journal";
  inline constexpr static float WIDTH = 750;
  inline constexpr static float HEIGHT = 550;

  QuestPanel()
      : Window(SCREEN_WIDTH / 2 - WIDTH / 2, SCREEN_HEIGHT * 0.2F, WIDTH, HEIGHT, 20,
               HEADER, KEY_J, sound::EMPTY_SOUND, sound::EMPTY_SOUND) {}
  void Draw() {
    side_panel.Draw(SCREEN_WIDTH - side_panel.body.width, SCALE(300));
    WINDOW_LOGIC()
    DrawWindow();
    questMenu.Draw(wholeWindow.x, wholeWindow.y + 25);
  }
  void Update() {
    PLAYER_QUESTS.Update();
    side_panel.Update();
    WINDOW_UPDATE();
    questMenu.Update();
  }
};

#endif  //MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
