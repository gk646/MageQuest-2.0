#ifndef MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_

#include "../elements/ExpandableMenu.h"

struct QuestSidePanel final : public Panel {
  static inline char HEADER[] = "Objectives";
  QuestSidePanel() : Panel(200, 150, 16, MINECRAFT_BOLD, HEADER) {}

  void DrawContent() noexcept final {
    if (PLAYER_QUESTS.HasActiveQuest()) {
      Util::      DrawOutlineText(MINECRAFT_BOLD, SCALE(16), PLAYER_QUESTS.activeQuest->name.c_str(),
                      body.x + SCALE(2), body.y + SCALE(35), 1, Colors::LightGrey,
                      Colors::black);
      Util::      DrawOutlineText(MINECRAFT_BOLD, SCALE(15),
                      PLAYER_QUESTS.activeQuest->GetActiveObjective().c_str(),
                      body.x + SCALE(20), body.y + SCALE(50), 1, Colors::LightGrey,
                      Colors::black);
    }else{
      Util::      DrawOutlineText(MINECRAFT_BOLD, SCALE(15),
                      "No active Quest",
                      body.x + SCALE(20), body.y + SCALE(50), 1, Colors::LightGrey,
                      Colors::black);
    }
  }
};

struct QuestPanel final : public Window {
  QuestSidePanel side_panel;
  ExpandableQuestMenu questMenu;
  static inline char HEADER[] = "Quest";
  inline constexpr static float WIDTH = 750;
  inline constexpr static float HEIGHT = 550;

  QuestPanel()
      : Window(SCREEN_WIDTH/2 - WIDTH/2, SCREEN_HEIGHT * 0.2F, WIDTH, HEIGHT, 20, HEADER,
               KEY_J),
        questMenu(WIDTH / 2, HEIGHT) {}
  void Draw() {
    side_panel.Draw(SCREEN_WIDTH - 230, SCALE(300));
    WINDOW_LOGIC()
    DrawWindow();
    questMenu.Draw(whole_window.x, whole_window.y);
  }
  void Update() {
    PLAYER_QUESTS.Update();
    side_panel.Update();
    WINDOW_UPDATE();
    questMenu.UpdateQuestBinding();
  }
};

#endif  //MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
