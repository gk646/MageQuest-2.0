#ifndef MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_

struct QuestSidePanel final : public Panel {
  static inline char HEADER[] = "Objectives";
  QuestSidePanel() : Panel(200, 150, 16, MINECRAFT_BOLD, HEADER) {}

  void DrawContent() noexcept final{
    if(PLAYER_QUESTS.HaveActiveQuest()){
      DrawOutlineText(MINECRAFT_BOLD, SCALE(15),PLAYER_QUESTS.activeQuest->name.c_str(), body.x+SCALE(2), body.y+SCALE(35), 1, Colors::LightGrey, Colors::black);
      DrawOutlineText(MINECRAFT_BOLD, SCALE(15),PLAYER_QUESTS.activeQuest->GetActiveObjective().c_str(), body.x+SCALE(15),body.y+SCALE(50), 1, Colors::LightGrey, Colors::black);
    }
  }
};

struct QuestPanel final : public Window {
  static inline char HEADER[] = "Quest";
  static constexpr int width = 500;

  QuestSidePanel side_panel;
  QuestPanel()
      : Window(SCREEN_WIDTH * 0.1F, SCREEN_HEIGHT * 0.2F, width, 500, 25, HEADER, KEY_J) {
  }
  void Draw() {
    side_panel.Draw(SCREEN_WIDTH - 230, SCALE(300));
  }
  void Update() {
    side_panel.Update();
    PLAYER_QUESTS.Update();
  }
};

#endif  //MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
