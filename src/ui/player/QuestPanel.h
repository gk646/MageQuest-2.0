#ifndef MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_

#include <raylib.h>
#include "../elements/Window.h"
#include "../../system/GlobalVariables.h"
#include "../../quest/QuestHandler.h"
#include "../elements/Panel.h"
#include "../../system/Definitions.h"
#include "../../resources/FontStorage.h"

struct QuestSidePanel final : public Panel {
  static inline char HEADER[] = "Objective";
  QuestSidePanel() : Panel(200, 150, 20, VARNISHED, HEADER) {}

  void DrawContent() noexcept final{

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

 private:
  void DrawSidePanel() noexcept {}
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_QUESTPANEL_H_
