#ifndef MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
struct QuestBox {
  TexturedButton button;
  const Quest& quest;
  bool clicked;
};
struct ExpandableQuestMenu {
  inline static char TRACK[] = "Track";
  inline static char UNTRACK[] = "Untrack";
  static inline float ELEMENT_HEIGHT = 50;
  static inline float INFO_BOX_OFFSET = 50;
  static inline float INFO_BOX_HEIGHT = 150;
  static inline float INFO_BOX_WIDTH;
  std::vector<QuestBox> items;
  RectangleR bounds;
  uint32_t prevSize = 0;
  ExpandableQuestMenu(float width, float height) : bounds(0, 0, width, height) {
    INFO_BOX_WIDTH = bounds.width - INFO_BOX_OFFSET;
  }
  void Draw(float x, float y) noexcept {
    Update(x, y);
    y += 25;
    x += bounds.width / 2;
    for (auto& box : items) {
      if (box.clicked) {
        DrawInfoPanel(x, y, box.quest);
      }
      if (box.button.Draw(x, y, TextAlign::LEFT)) {
        box.clicked = !box.clicked;
      }
      DrawRightAlignedText(MINECRAFT_REGULAR, 16,
                           std::to_string(box.quest.questLevel).c_str(),
                           x + box.button.bounds.width * 0.2F, y + 18,
                           GetLevelRangeColor(box.quest.questLevel));
      y += ELEMENT_HEIGHT + 1;
      if (box.clicked) {
        y += INFO_BOX_HEIGHT + 1;
      }
    }
  }
  void UpdateQuestBinding(bool force = false) noexcept {
    if (prevSize != PLAYER_QUESTS.quests.size() || force) {
      items.clear();
      for (const auto q : PLAYER_QUESTS.quests) {
        items.emplace_back(TexturedButton{bounds.width, ELEMENT_HEIGHT, q->name.c_str(),
                                          15, textures::ui::questpanel::questBox,
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
  inline static Color GetLevelRangeColor(uint8_t questLevel) noexcept {
    auto diff = PLAYER_STATS.level - questLevel;
    if (diff >= 0) return Colors::StatGreen;
    else if (diff >= -3)
      return Colors::notificationOrange;
    else
      return Colors::Red;
  }
  inline void DrawInfoPanel(float x, float y, const Quest& quest) const noexcept {
    //normalize values to Left Top corner of info box
    x -= (bounds.width / 2 - INFO_BOX_OFFSET / 2);
    y += +ELEMENT_HEIGHT - 5;
    DrawRectangleRounded({x, y, bounds.width - INFO_BOX_OFFSET, INFO_BOX_HEIGHT}, 0.1,
                         ROUND_SEGMENTS, Colors::mediumLightGrey);
    if (!PLAYER_QUESTS.HasActiveQuest()) {
      if (Button::Draw({x + INFO_BOX_WIDTH - 30, y + INFO_BOX_HEIGHT - 20, 30, 20},
                       "Track", "")) {
        PLAYER_QUESTS.SetActive(quest.id);
      }
    } else if(PLAYER_QUESTS.activeQuest->id == quest.id) {
      if (Button::Draw({x + INFO_BOX_WIDTH - 30, y + INFO_BOX_HEIGHT - 20, 30, 20},
                       "Untrack", "")) {
        PLAYER_QUESTS.ClearActive();
      }
    }else{

    }
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
