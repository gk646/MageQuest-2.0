#ifndef MAGE_QUEST_SRC_UI_SETTINGSMENU_H_
#define MAGE_QUEST_SRC_UI_SETTINGSMENU_H_

struct SettingsMenu {
  int targetFPS =  120;
  bool showFPS = false;

  int selectedResolution = 0;
  const char* resolutions = "800x600;1280x720;1920x1080;2560x1440;3840x2160";

  int baseWidth = 140;
  int baseHeight = 30;
  int offsetX = 70;
  int offsetY = 40;
  bool fps_checkbox = false;
  void draw() noexcept {
    // Draw Background Rectangle
    DrawRectanglePro(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {0, 0}, 0, ColorAlpha(GRAY, 0.7));

    // Initialize UI scaling and position
    float scaledWidth = baseWidth * UI_SCALE;
    float scaledHeight = baseHeight * UI_SCALE;
    float startX = SCREEN_WIDTH / 2 - scaledWidth / 2;
    float startY = SCREEN_HEIGHT / 2 - scaledHeight * 3;
    float verticalSpacing = 35 * UI_SCALE;

    // Draw Window size combo box
    Rectangle resolutionBounds = {startX, startY, scaledWidth, scaledHeight};
    int newSelectedResolution = GuiComboBox(resolutionBounds, resolutions, selectedResolution,
                                            IsWindowFullscreen() ? STATE_DISABLED : STATE_NORMAL);
    if (newSelectedResolution != selectedResolution) {
      selectedResolution = newSelectedResolution;
      setResolution(selectedResolution);
    }

    // Draw UI scale slider
    startY += verticalSpacing;
    UI_SCALE = GuiSlider({startX, startY, scaledWidth, scaledHeight}, "UI Scale", nullptr, UI_SCALE, 0.7f, 2.0f);

    // Draw Fullscreen checkbox
    startY += verticalSpacing;
    bool fullscreen = GuiCheckBox({startX, startY, 20 * UI_SCALE, 20 * UI_SCALE}, "Fullscreen", IsWindowFullscreen());
    if (fullscreen != IsWindowFullscreen()) {
      ToggleFullscreen();
      if (fullscreen) {
        SCREEN_WIDTH = GetMonitorWidth(0);
        SCREEN_HEIGHT = 1080;
        SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
      } else {
        setResolution(selectedResolution);
      }
    }

    CAMERA_X = SCREEN_WIDTH / 2;
    CAMERA_Y = SCREEN_HEIGHT / 2;

    // Draw FPS controls
    startY += verticalSpacing;
    showFPS = GuiCheckBox({startX, startY, 20 * UI_SCALE, 20 * UI_SCALE}, "Show FPS", showFPS);

    // Draw FPS Target slider
    startY += verticalSpacing;
    targetFPS = static_cast<int>(GuiSlider({startX, startY, scaledWidth, scaledHeight}, "FPS Target", nullptr,
                                           static_cast<float>(targetFPS), 60, 240));
    SetTargetFPS(targetFPS);
  }
  void setResolution(int resolution) const {
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
};
#endif  //MAGE_QUEST_SRC_UI_SETTINGSMENU_H_
