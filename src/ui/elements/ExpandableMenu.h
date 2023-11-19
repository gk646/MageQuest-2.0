#ifndef MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_

struct QuestBox {
  TexturedButton button;
  Quest& quest;
  bool clicked;
};

struct ExpandableQuestMenu final : public Content {
  static inline constexpr float ELEMENT_HEIGHT = 50;
  static inline constexpr float INFO_BOX_OFFSET = 50;
  static inline constexpr float INFO_BOX_HEIGHT = 150;
  static inline float INFO_BOX_WIDTH;
  std::vector<QuestBox> items{};
  RectangleR bounds;
  QuestSidePanel* panel;
  float lowerLimit = 0, upperLimit = 0;
  uint32_t prevSize = 0;
  ExpandableQuestMenu(float width, float height, QuestSidePanel* panel)
      : bounds(0, 0, width, height), panel(panel) {
    INFO_BOX_WIDTH = bounds.width - INFO_BOX_OFFSET;
  }
  void Draw(RectangleR cBounds, float scrollOffset) noexcept final {
    UpdateImpl(cBounds.x, cBounds.y);
    UpdateLimits(cBounds, scrollOffset);
    //BeginTextureMode(FIRST_LAYER_BUFFER);
    //ClearBackground(BLANK);
    //DrawRectangleRounded({cBounds.x, cBounds.y, bounds.width, GetHeight()}, 0.1F, 30, RED);
    for (auto& box : items) {
      box.button.bounds.width = cBounds.width;
      if (box.clicked &&
          IsInBounds(cBounds.y + (ELEMENT_HEIGHT - 5), INFO_BOX_HEIGHT - 5)) {
        DrawInfoPanel(cBounds.x, cBounds.y + ELEMENT_HEIGHT - 5, box.quest);
        //DrawQuestText(box.quest, bounds.x + bounds.width / 2.0F, bounds.y);
      }
      if (IsInBounds(cBounds.y, ELEMENT_HEIGHT) &&
          box.button.Draw(cBounds.x, cBounds.y, Alignment::LEFT, Alignment::LEFT)) {
        box.clicked = !box.clicked;
      }
      if (IsInBounds(cBounds.y, ELEMENT_HEIGHT)) {
        Util::DrawRightAlignedText(
            MINECRAFT_BOLD, 17, std::to_string(box.quest.questLevel).c_str(),
            cBounds.x + box.button.bounds.width * 0.8F, cBounds.y + 16,
            GetLevelRangeColor(box.quest.questLevel));
      }
      cBounds.y += ELEMENT_HEIGHT + 1;
      if (box.clicked) {
        cBounds.y += INFO_BOX_HEIGHT + 1;
      }
    }
    //EndTextureMode();
    //DrawTextureFlipped(FIRST_LAYER_BUFFER.texture, 0, 0, true);
  }
  void Update() noexcept final { UpdateQuestBinding(); }
  [[nodiscard]] float GetWidth() const noexcept final { return 5; }
  [[nodiscard]] float GetHeight() const noexcept final {
    float height = 0;
    for (const auto& box : items) {
      height += ELEMENT_HEIGHT + 1;
      if (box.clicked) height += INFO_BOX_HEIGHT + 1;
    }
    return height;
  }
  //Keeps the tabs up to date with the state of the PLAYER_QUESTS quest handler
  void UpdateQuestBinding() noexcept {
    if (prevSize != PLAYER_QUESTS.quests.size() || PLAYER_QUESTS.updateHappened) {
      items.clear();
      for (const auto q : PLAYER_QUESTS.quests) {
        if (q->hidden || q->state == QuestState::COMPLETED) continue;
        items.emplace_back(TexturedButton{bounds.width, ELEMENT_HEIGHT, q->name, 15,
                                          textures::ui::questpanel::questBox,
                                          textures::ui::questpanel::questBoxHovered,
                                          textures::ui::questpanel::questBoxPressed, 255,
                                          "Expand Quest"},
                           *q);
      }
      PLAYER_QUESTS.updateHappened = false;
      prevSize = PLAYER_QUESTS.quests.size();
    }
  }

 private:
  //Method to update limits
  inline void UpdateLimits(RectangleR& cBounds, float offSet) noexcept {
    lowerLimit = cBounds.y;
    upperLimit = cBounds.y + cBounds.height;
    cBounds.y -= offSet;
    cBounds.y += 5;
  }
  //Returns true if within the content bounds
  [[nodiscard]] inline bool IsInBounds(float y, float height) const noexcept {
    return y >= lowerLimit && y + height <= upperLimit;
  }
  inline void UpdateImpl(float x, float y) noexcept {
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
        PLAYER_QUESTS.SetAsActiveQuest((Quest*)&quest);
        panel->expanded = true;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_EXPANDABLEMENU_H_
