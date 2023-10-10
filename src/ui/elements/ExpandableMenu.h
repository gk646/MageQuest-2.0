#ifndef MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
struct QuestBox {
  TexturedButton button;
  const Quest& quest;
  bool clicked;
};
struct ExpandableQuestMenu {
  static inline float ELEMENT_HEIGHT = 50;
  static inline float INFO_BOX_OFFSET = 50;
  static inline float INFO_BOX_HEIGHT = 150;
  std::vector<QuestBox> items;
  RectangleR bounds;
  uint32_t prevSize = 0;
  ExpandableQuestMenu(float width, float height) : bounds(0, 0, width, height) {}
  void Draw(float x, float y) noexcept {
    Update(x, y);
    y += 25;
    x += bounds.width / 2;
    for (auto& box : items) {
      if (box.button.Draw(x, y, TextAlign::LEFT)) {
        box.clicked = !box.clicked;
      }
      y += 50;
      if (box.clicked) {

        DrawRectangleProFast(x - bounds.width / 2 + INFO_BOX_OFFSET / 2, y,
                             bounds.width - INFO_BOX_OFFSET, INFO_BOX_HEIGHT, BLACK);
        y += INFO_BOX_HEIGHT;
      }
    }
  }
  void UpdateQuestBinding(bool force = false) noexcept {
    if (prevSize != PLAYER_QUESTS.quests.size() || force) {
      items.clear();
      for (const auto q : PLAYER_QUESTS.quests) {
        items.emplace_back(TexturedButton{bounds.width, ELEMENT_HEIGHT, q->name.c_str(),
                                          14, textures::ui::questpanel::questBox,
                                          textures::ui::questpanel::questBoxHovered,
                                          textures::ui::questpanel::questBoxPressed},
                           *q);
      }
      prevSize = PLAYER_QUESTS.quests.size();
    }
  }

 private:
  inline void Update(float x, float y) noexcept {
    bounds.x = x;
    bounds.y = y;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
