#ifndef MAGEQUEST_SRC_UI_ELEMENTS_QUESTTEXT_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_QUESTTEXT_H_
struct QuestText final : public Content {
  ExpandableQuestMenu* questMenu;
  float width, lowerLimit = 0, upperLimit = 0, contentHeight = 0;
  QuestText(ExpandableQuestMenu* questMenu, float width)
      : questMenu(questMenu), width(width) {}

  inline void Draw(RectangleR bounds, float scrollOffset) noexcept final {
    UpdateLimits(bounds, scrollOffset);
    Quest* quest = nullptr;

    for (const auto& box : questMenu->items) {
      if (box.clicked) {
        quest = &box.quest;
      }
    }
    contentHeight = 0;
    if (!quest) return;
    float offSetY = 0;
    for (const auto& text : quest->pastDialogue) {
      int lineBreaks = 0;
      auto wrappedText = Util::WrapText(text, bounds.width, MINECRAFT_REGULAR, 15, &lineBreaks);
      if (IsInBounds(bounds.y+ offSetY, lineBreaks * 15+15)) {
        DrawTextExR(MINECRAFT_REGULAR, wrappedText.c_str(), {bounds.x + 2, bounds.y+ offSetY}, 15, 0.5F,
                    Colors::darkBackground);
      }
      offSetY += lineBreaks * 15 +15;
    }
    contentHeight = offSetY;
  }
  inline void Update() noexcept final {}
  inline float GetWidth() const noexcept final { return 0; }
  inline float GetHeight() const noexcept final { return contentHeight; }

 private:
  //Method to update limits
  inline void UpdateLimits(RectangleR& cBounds, float offSet) noexcept {
    lowerLimit = cBounds.y;
    upperLimit = cBounds.y + cBounds.height;
    cBounds.y -= offSet;
    cBounds.y += 5;
  }
  [[nodiscard]] inline bool IsInBounds(float y, float height) const noexcept {
    return y >= lowerLimit && y + height <= upperLimit;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_QUESTTEXT_H_
