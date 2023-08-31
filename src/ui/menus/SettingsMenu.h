#ifndef DND_SRC_UI_SETTINGSMENU_H_
#define DND_SRC_UI_SETTINGSMENU_H_

#include "../Component.h"

struct SettingsMenu : public Component {
  int* screen_height;
  int* screen_width;
  float* ui_scale;
  int* targetFPS;
  bool* showFPS;

  int selectedResolution = 0;
  const char* resolutions = "800x600;1280x720;1920x1080;2560x1440;3840x2160";

  int baseWidth = 140;
  int baseHeight = 30;
  int offsetX = 70;
  int offsetY = 40;
  bool fps_checkbox = false;
  SettingsMenu(int* screen_width, int* screen_height, float* ui_scale, int* targetFPS,
               bool* showFPS)
      : screen_width(screen_width),
        screen_height(screen_height),
        ui_scale(ui_scale),
        targetFPS(targetFPS),
        showFPS(showFPS) {}
  ~SettingsMenu() { delete resolutions; }
  void draw(float ui_scale_old, int x, int y) final {
    DrawRectanglePro({0, 0, (float)x, (float)y}, {0, 0}, 0, ColorAlpha(GRAY, 0.7));
    float scaledWidth = baseWidth * ui_scale_old;
    float scaledHeight = baseHeight * ui_scale_old;

    float startX = x / 2 - scaledWidth / 2;
    float startY = y / 2 - scaledHeight * 3;

    // Window size combo box
    Rectangle resolutionBounds = {startX, startY, scaledWidth, scaledHeight};
    int newSelectedResolution = 0;
    if (IsWindowFullscreen()) {
      newSelectedResolution =
          GuiComboBox(resolutionBounds, resolutions, selectedResolution, STATE_DISABLED);
    } else {
      newSelectedResolution =
          GuiComboBox(resolutionBounds, resolutions, selectedResolution, STATE_NORMAL);
    }
    if (newSelectedResolution != selectedResolution) {
      selectedResolution = newSelectedResolution;
      setResolution(selectedResolution);
    }

    // UI scale slider
    startY += 35 * ui_scale_old;
    Rectangle scaleSliderBounds = {startX, startY, scaledWidth, scaledHeight};
    *ui_scale = GuiSlider(scaleSliderBounds, "UI Scale", nullptr, *ui_scale, 0.7f, 2.0f);

    bool fullscreen = IsWindowFullscreen();
    startY += 35 * ui_scale_old;
    Rectangle checkboxRect = {startX, startY, 20 * ui_scale_old, 20 * ui_scale_old};
    fullscreen = GuiCheckBox(checkboxRect, "Fullscreen", fullscreen);
    if (fullscreen) {
      if (!IsWindowFullscreen()) {
        *screen_width = GetMonitorWidth(0);
        *screen_height = 1080;
        SetWindowSize(*screen_width, *screen_height);
        ToggleFullscreen();
      }

    } else {
      if (IsWindowFullscreen()) {
        ToggleFullscreen();
        setResolution(selectedResolution);
      }
    }

    //FPS
    startY += 35 * ui_scale_old;
    Rectangle fps_checkbox_bound = {startX, startY, 20 * ui_scale_old, 20 * ui_scale_old};
    fps_checkbox = GuiCheckBox(fps_checkbox_bound, "Show FPS", fps_checkbox);
    if (fps_checkbox) {
      if (!*showFPS) {
        *showFPS = true;
      }
    } else {
      if (*showFPS) {
        *showFPS = false;
      }
    }

    startY += 35 * ui_scale_old;
    Rectangle fpsSliderBounds = {startX, startY, scaledWidth, scaledHeight};
    *targetFPS = (int)GuiSlider(fpsSliderBounds, "FPS Target", nullptr, (float)*targetFPS, 60, 240);
    SetTargetFPS(*targetFPS);
  }
  void setResolution(int resolution) const {
    switch (resolution) {
      case 0:
        SetWindowSize(800, 600);
        *screen_width = 800;
        *screen_height = 600;
        break;
      case 1:
        SetWindowSize(1280, 720);
        *screen_width = 1280;
        *screen_height = 720;
        break;
      case 2:
        SetWindowSize(1920, 1080);
        *screen_width = 1920;
        *screen_height = 1080;
        break;
      case 3:
        if (GetMonitorWidth(0) >= 2560 && GetMonitorHeight(0) >= 1440) {
          SetWindowSize(2560, 1440);
          *screen_width = 2560;
          *screen_height = 1440;
        }
        break;
      case 4:
        if (GetMonitorWidth(0) >= 3840 && GetMonitorHeight(0) >= 2160) {
          SetWindowSize(3840, 2160);
          *screen_width = 3840;
          *screen_height = 2160;
        }
        break;
      default:
        break;
    }
    SetWindowPosition(0, 30);
  }
};
#endif  //DND_SRC_UI_SETTINGSMENU_H_
