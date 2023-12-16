#ifndef MAGE_QUEST_SRC_UI_SETTINGSMENU_H_
#define MAGE_QUEST_SRC_UI_SETTINGSMENU_H_

struct SettingsMenu {
  int selectedResolution = 0;
  inline static const char* resolutions =
      "800x600;1280x720;1920x1080;2560x1440;3840x2160";
  inline static constexpr int baseWidth = 140;
  inline static constexpr int baseHeight = 30;
  inline static constexpr int offsetX = 70;
  inline static constexpr int offsetY = 40;
  inline static constexpr int VERTICAL_SPACING = 35;
  inline static constexpr int KEY_ASSIGN_WIDTH = 350;
  inline static constexpr int KEY_ASSIGN_HEIGHT = 60;
  inline static bool keyAssignMode = false;
  inline static bool wrongAssign = false;
  inline static int abilityNum = -1;

  void Draw() noexcept {
    auto& stats = GAME_SETTINGS;
    DrawBasicBackground();

    float startX = SCREEN_WIDTH / 2 - baseWidth / 2;
    float startY = SCREEN_HEIGHT / 2 - baseHeight * 14;

    DrawVideoOptions(stats, startX, startY);
    DrawPerformanceSettings(stats, startX, startY);
    DrawAudioSettings(stats, startX, startY);
    DrawGameplaySettings(stats, startX, startY);
    DrawKeybindSettings(stats, startX, startY);

    DrawKeyAssignWindow(stats);
  }
  static void SetFullScreen() {
    SetWindowPosition(0, 0);
    SCREEN_WIDTH = (float)GetMonitorWidth(0);
    SCREEN_HEIGHT = (float)GetMonitorHeight(0);
    SetWindowSize((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);
    UnloadRenderTexture(FIRST_LAYER_BUFFER);
    FIRST_LAYER_BUFFER = LoadRenderTexture((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);

    ToggleFullscreen();
  }
  void ApplySettings() {
    //TODO add more resolutions and add scaling
    // SetResolution(selectedResolution);
    ToggleFullScreen(GAME_SETTINGS.isFullScreen);
    SetTargetFPS(GAME_SETTINGS.targetFPS);
    SetMasterVolume(GAME_SETTINGS.masterVolume);
    UpdateRoundSegments(GAME_SETTINGS.fastUI);
  }

 private:
  inline static void DrawBasicBackground() {
    DrawRectanglePro(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, 0, ColorAlpha(GRAY, 0.7));
  }
  void DrawVideoOptions(GameSettings& stats, float startX, float& startY) {
    GuiLabel({startX - 10, startY, baseWidth, baseHeight}, "Video Settings");
    startY += VERTICAL_SPACING;

    int newSelectedResolution = GuiComboBox(
        {startX, startY, baseWidth, baseHeight}, resolutions, selectedResolution,
        IsWindowFullscreen() ? STATE_DISABLED : STATE_NORMAL);
    if (newSelectedResolution != selectedResolution) {
      selectedResolution = newSelectedResolution;
      SetResolution(selectedResolution);
    }
    startY += VERTICAL_SPACING;

    stats.isFullScreen =
        GuiCheckBox({startX, startY, 20, 20}, "Fullscreen", IsWindowFullscreen());
    ToggleFullScreen(stats.isFullScreen);
    startY += VERTICAL_SPACING;

    stats.showFPS = GuiCheckBox({startX, startY, 20, 20}, "Show FPS", stats.showFPS);
    startY += VERTICAL_SPACING;

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%d", stats.targetFPS);
    stats.targetFPS = static_cast<int16_t>(
        GuiSlider({startX, startY, baseWidth, baseHeight}, "FPS Target", TEXT_BUFFER,
                  static_cast<float>(stats.targetFPS), 60, 240));
    SetTargetFPS(stats.targetFPS);

    startY += VERTICAL_SPACING;
    startY += VERTICAL_SPACING;
  }
  static void DrawPerformanceSettings(GameSettings& stats, float startX, float& startY) {
    GuiLabel({startX - 10, startY, baseWidth, baseHeight}, "Graphic Settings");
    startY += VERTICAL_SPACING;

    GAME_SETTINGS.fastUI =
        GuiCheckBox({startX, startY, 20, 20}, "Fast UI", GAME_SETTINGS.fastUI);
    UpdateRoundSegments(GAME_SETTINGS.fastUI);
    startY += VERTICAL_SPACING;

    stats.disableDynamicLighting =
        GuiCheckBox({startX, startY, 20, 20}, "Disable Dynamic Lighting",
                    stats.disableDynamicLighting);
    startY += VERTICAL_SPACING;
    startY += VERTICAL_SPACING;
  }
  static void DrawAudioSettings(GameSettings& stats, float startX, float& startY) {
    GuiLabel({startX - 10, startY, baseWidth, baseHeight}, "Audio Settings");
    startY += VERTICAL_SPACING;

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%d", (int)(stats.masterVolume * 100));
    stats.masterVolume =
        GuiSlider({startX, startY, baseWidth, baseHeight}, "Master Volume", TEXT_BUFFER,
                  stats.masterVolume, 0, 1);
    SetMasterVolume(stats.masterVolume);
    startY += VERTICAL_SPACING;

    snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "%d", (int)(stats.musicVolume * 100));
    stats.musicVolume = GuiSlider({startX, startY, baseWidth, baseHeight}, "Music Volume",
                                  TEXT_BUFFER, stats.musicVolume, 0, 1);
    //TODO adjust music volume
    startY += VERTICAL_SPACING;

    startY += VERTICAL_SPACING;
  }
  static void DrawGameplaySettings(GameSettings& stats, float startX, float& startY) {
    GuiLabel({startX - 10, startY, baseWidth, baseHeight}, "Gameplay Settings");
    startY += VERTICAL_SPACING;

    stats.showHealthNumbers = GuiCheckBox(
        {startX, startY, 20, 20}, "Show Enemy Health Numbers", stats.showHealthNumbers);
    startY += VERTICAL_SPACING;

    startY += VERTICAL_SPACING;
  }
  static void DrawKeybindSettings(GameSettings& stats, float startX, float& startY) {
    GuiLabel({startX - 10, startY, baseWidth, baseHeight}, "Keybinds");
    startY += VERTICAL_SPACING;

    for (int i = 1; i < 5; i++) {
      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Ability %d: %s", i,
               Util::GetKeyName(stats.playerKeybinds[i]));
      GuiLabel({startX - 60, startY, 80, 20}, TEXT_BUFFER);

      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Assign new key:");
      if (GuiButton({startX + 35, startY - 5, baseWidth, baseHeight}, TEXT_BUFFER)) {
        keyAssignMode = true;
        abilityNum = i;
      }
      startY += VERTICAL_SPACING;
    }
    startY += VERTICAL_SPACING;
  }

  //Helper methods
 private:
  static inline void DrawKeyAssignWindow(GameSettings& stats) {
    if (abilityNum == -1) keyAssignMode = false;
    if (keyAssignMode) {
      DrawRectangleProFast(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Colors::LightGreyAlpha);
      DrawRectangleRounded(
          {(SCREEN_WIDTH - KEY_ASSIGN_WIDTH) / 2, (SCREEN_HEIGHT - KEY_ASSIGN_HEIGHT) / 2,
           KEY_ASSIGN_WIDTH, KEY_ASSIGN_HEIGHT},
          0.1F, stats.roundSegments, Colors::LightGrey);
      DrawRectangleRoundedLines(
          {(SCREEN_WIDTH - KEY_ASSIGN_WIDTH) / 2, (SCREEN_HEIGHT - KEY_ASSIGN_HEIGHT) / 2,
           KEY_ASSIGN_WIDTH, KEY_ASSIGN_HEIGHT},
          0.1F, stats.roundSegments, 2, Colors::darkBackground);

      snprintf(TEXT_BUFFER, TEXT_BUFFER_SIZE, "Press a key to assign to Ability %d...",
               abilityNum);

      Util::DrawCenteredText(MINECRAFT_BOLD, 17, TEXT_BUFFER, SCREEN_WIDTH / 2,
                             (SCREEN_HEIGHT - KEY_ASSIGN_HEIGHT) / 2 + 10,
                             Colors::darkBackground);

      if (wrongAssign) {
        Util::DrawCenteredText(
            MINECRAFT_ITALIC, 15, "1-9 or A-Z and unique. Excluded: W,A,S,D,C,M,N,P,J",
            SCREEN_WIDTH / 2, (SCREEN_HEIGHT - KEY_ASSIGN_HEIGHT) / 2 + 30,
            Colors::darkBackground);
      }

      if (IsKeyDown(KEY_ESCAPE)) {
        keyAssignMode = false;
        ConsumeKeyPresses();
        return;
      }
      int keyPressed = GetKeyPressed();
      keyPressed = std::toupper(keyPressed);

      if (keyPressed > 0) {
        if (CheckValidKey(keyPressed) && Util::GetKeyName(keyPressed) != nullptr) {
          stats.playerKeybinds[abilityNum] = (int16_t)keyPressed;
          keyAssignMode = false;
          wrongAssign = false;
        } else {
          wrongAssign = true;
        }
      }
      ConsumeKeyPresses();
    }
  }
  inline void ToggleFullScreen(bool currentButtonVal) const {
    if (currentButtonVal != IsWindowFullscreen()) {
      if (currentButtonVal) {
        SetFullScreen();
      } else {
        ToggleFullscreen();
        SetResolution(selectedResolution);
      }
    }
  }
  static void SetResolution(int resolution) noexcept {
    switch (resolution) {
      case 0:
        SetWindowSize(800, 600);
        SCREEN_WIDTH = 800;
        SCREEN_HEIGHT = 600;
        break;
      case 1:
        SetWindowSize(1280, 720);
        SCREEN_WIDTH = 1280;
        SCREEN_HEIGHT = 720;
        break;
      case 2:
        SetWindowSize(1920, 1080);
        SCREEN_WIDTH = 1920;
        SCREEN_HEIGHT = 1080;
        break;
      case 3:
        if (GetMonitorWidth(0) >= 2560 && GetMonitorHeight(0) >= 1440) {
          SetWindowSize(2560, 1440);
          SCREEN_WIDTH = 2560;
          SCREEN_HEIGHT = 1440;
        }
        break;
      case 4:
        if (GetMonitorWidth(0) >= 3840 && GetMonitorHeight(0) >= 2160) {
          SetWindowSize(3840, 2160);
          SCREEN_WIDTH = 3840;
          SCREEN_HEIGHT = 2160;
        }
        break;
      default:
        break;
    }
    SetWindowPosition(0, 30);
  }
  static void UpdateRoundSegments(bool val) noexcept {
    if (val) {
      GAME_SETTINGS.roundSegments = 15;
    } else {
      GAME_SETTINGS.roundSegments = 30;
    }
  }
  inline static bool CheckValidKey(int key) noexcept {
    //Prebound keys not available for assignment
    if (key == KEY_C || key == KEY_J || key == KEY_B || key == KEY_N || key == KEY_M ||
        key == KEY_P || key == KEY_W || key == KEY_A || key == KEY_S || key == KEY_D)
      return false;

    for (int i = 1; i < 5; i++) {
      if (key == GAME_SETTINGS.playerKeybinds[i]) {
        return false;
      }
    }
    return true;
  }
};
#endif  //MAGE_QUEST_SRC_UI_SETTINGSMENU_H_
