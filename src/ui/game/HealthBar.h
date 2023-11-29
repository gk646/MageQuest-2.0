#ifndef MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
#define MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_

struct HealthBar {
  inline static Monster* bossPtrs[3];
  inline static int8_t bossCount = 0;
  int16_t delay = 0;
  uint8_t width = 50;
  uint8_t height = 10;
  bool isBoss = false;
  explicit HealthBar(int width) noexcept : width(width) {}
  inline static void UpdateStatic() noexcept { bossCount = 0; }
  inline static void DrawStatic() noexcept {
    for (int i = 0; i < bossCount; i++) {
      if (!bossPtrs[i]) break;
      DrawBossBar(bossPtrs[i], i);
    }
  }
  void Draw(Monster* self) const noexcept {
    if (isBoss) {
      RegisterBossBar(self);
    } else {
      DrawNormal(self);
    }
  };
  void Update() { delay--; }
  void Show() { delay = 300; }

 private:
  inline static void RegisterBossBar(Monster* self) {
    if (bossCount >= 3) return;
    bossPtrs[bossCount] = self;
    bossCount++;
  };
  inline static void DrawBossBar(const Monster* self, int i);
  void DrawNormal(const Monster* self) const noexcept;
};
#endif  //MAGE_QUEST_SRC_UI_GAME_HEALTHBAR_H_
