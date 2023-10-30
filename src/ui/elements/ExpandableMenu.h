#ifndef MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_

struct QuestBox {
  TexturedButton button;
  const Quest& quest;
  bool clicked;
};

struct ExpandableQuestMenu final : public Content {
  static inline float ELEMENT_HEIGHT = 50;
  static constexpr inline float INFO_BOX_OFFSET = 50;
  static constexpr inline float INFO_BOX_HEIGHT = 150;
  static inline float INFO_BOX_WIDTH;
  std::vector<QuestBox> items;
  RectangleR bounds;
  uint32_t prevSize = 0;
  ExpandableQuestMenu(float width, float height) : bounds(0, 0, width, height) {
    INFO_BOX_WIDTH = bounds.width - INFO_BOX_OFFSET;
  }
  void Draw(RectangleR cBounds) noexcept final {
    Update(cBounds.x, cBounds.y);
    cBounds.y += 25;
    for (auto& box : items) {
      box.button.bounds.width = cBounds.width;
      if (box.clicked) {
        DrawInfoPanel(cBounds.x, cBounds.y, box.quest);
      }
      if (box.button.Draw(cBounds.x, cBounds.y, Alignment::LEFT, Alignment::LEFT)) {
        box.clicked = !box.clicked;
      }
      Util::DrawRightAlignedText(
          MINECRAFT_BOLD, 17, std::to_string(box.quest.questLevel).c_str(),
          cBounds.x + box.button.bounds.width * 0.8F, cBounds.y + 16,
          GetLevelRangeColor(box.quest.questLevel));
      cBounds.y += ELEMENT_HEIGHT + 1;
      if (box.clicked) {
        cBounds.y += INFO_BOX_HEIGHT + 1;
      }
    }
  }
  void Update() noexcept final { UpdateQuestBinding(); }
  int GetWidth() const noexcept final { return 5; }
  int GetHeight() const noexcept final {
    if (items.size() > 0) {
      return items[0].clicked ? 1000 : 0;
    }
    return 0;
  }
  //Keeps the tabs up to date with the state of the PLAYER_QUESTS quest handler
  void UpdateQuestBinding() noexcept {
    if (prevSize != PLAYER_QUESTS.quests.size() || PLAYER_QUESTS.updateHappened) {
      items.clear();
      for (const auto q : PLAYER_QUESTS.quests) {
        if (q->hidden) continue;
        items.emplace_back(TexturedButton{bounds.width, ELEMENT_HEIGHT, q->name, 15,
                                          textures::ui::questpanel::questBox,
                                          textures::ui::questpanel::questBoxHovered,
                                          textures::ui::questpanel::questBoxPressed},
                           *q);
      }
      PLAYER_QUESTS.updateHappened = false;
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
    y += +ELEMENT_HEIGHT - 5;
    x += INFO_BOX_OFFSET / 2;
    DrawRectangleRounded({x, y, bounds.width - INFO_BOX_OFFSET, INFO_BOX_HEIGHT}, 0.1,
                         ROUND_SEGMENTS, Colors::mediumLightGrey);

    auto desc =
        Util::WrapText(quest.description, INFO_BOX_WIDTH - 10, MINECRAFT_BOLD, 15);
    DrawTextExR(MINECRAFT_BOLD, desc.c_str(), {x + 5, y + 5}, 15, 0.5,
                Colors::darkBackground);

    if (PLAYER_QUESTS.HasActiveQuest() && PLAYER_QUESTS.activeQuest->id == quest.id) {
      if (Button::Draw({x + INFO_BOX_WIDTH - 60, y + INFO_BOX_HEIGHT - 40, 40, 30},
                       "Untrack", "")) {
        PLAYER_QUESTS.RemoveActiveQuest();
      }
    } else {
      if (Button::Draw({x + INFO_BOX_WIDTH - 60, y + INFO_BOX_HEIGHT - 40, 40, 30},
                       "Track", "")) {
        PLAYER_QUESTS.SetAsActiveQuest(quest.id);
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
