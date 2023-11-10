#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLPANEL_H_

struct SkillPanel final : public Window {
  inline static constexpr float WINDOW_WIDTH = 610;
  inline static constexpr float WINDOW_HEIGHT = 500;
  inline static constexpr float SKILL_WIDTH = 60;
  inline static constexpr int SKILLS_PER_ROW = (WINDOW_WIDTH - 10) / SKILL_WIDTH;
  std::string inputField;
  SkillPanel()
      : Window((SCREEN_WIDTH - WINDOW_WIDTH) / 2, (SCREEN_HEIGHT - WINDOW_HEIGHT) / 2,
               WINDOW_WIDTH, WINDOW_HEIGHT, 20, "Skills", KEY_P, sound::EMPTY_SOUND,
               sound::EMPTY_SOUND) {}

 public:
  void Draw() noexcept {
    WINDOW_LOGIC()
    DrawWindow();
    DrawSkills();
  }
  void Update() noexcept {
    WINDOW_UPDATE();
    for (auto& slot : PLAYER_SKILLS) {
      slot->Update();
    }
  }

 private:
  inline void DrawSkills() noexcept {
    SkillSlot* toolTip = nullptr;
    auto topLeft = GetWindowPos();
    topLeft.x() += 5;
    topLeft.y() += 35;
    //GuiTextBox({topLeft.x() + wholeWindow.width - 200, topLeft.y(), 150, 15},inputField.data(), 50, true);
    for (int i = 1; i <= PLAYER_SKILLS.size(); i++) {
      if (PLAYER_SKILLS[i - 1]->Draw(topLeft.x(), topLeft.y())) {
        toolTip = PLAYER_SKILLS[i - 1];
      }
      topLeft.x() += SKILL_WIDTH;
      if (i % (SKILLS_PER_ROW ) == 0) {
        topLeft.y() += 60;
        topLeft.x() -= SKILLS_PER_ROW * SKILL_WIDTH;
      }
    }
    if (toolTip) {
      toolTip->DrawToolTip();
    }
  }
};

void AddSkill(Skill* skill) noexcept {
  PLAYER_SKILLS.push_back(new SkillSlot(0, 0, skill));
}
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_SKILLPANEL_H_
