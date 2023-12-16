#ifndef MAGEQUEST_SRC_UI_WINDOW_H_
#define MAGEQUEST_SRC_UI_WINDOW_H_

#include "ToolTip.h"
#include "TextCell.h"
#include "Button.h"
#include "ExpandablePanel.h"
#include "TexturedButton.h"
#include "UIHitbox.h"

struct Window {
  RectangleR wholeWindow;
  RectangleR header_bar;
  Vector2 lastMousePos = {0};
  Vector2 basePosition;
  const char* header_text;
  const Sound& openSound;
  const Sound& closeSound;
  int windowOpenKey;
  float fontSize = 17;
  bool isDragged = false;
  bool isWindowOpen = false;
  bool isHeaderHovered = false;
  Window(float start_x, float start_y, float width, float height, float header_height,
         const char* header_text, int open_key, const Sound& openSound,
         const Sound& closeSound)
      : wholeWindow(start_x, start_y, width, height),
        header_bar(start_x, start_y + 2, width, header_height),
        header_text(header_text),
        windowOpenKey(open_key),
        basePosition(start_x, start_y),
        openSound(openSound),
        closeSound(closeSound) {}

 public:
  //Opens the window
  inline void OpenWindow() noexcept {
    if (isWindowOpen) return;
    isWindowOpen = true;
    PlaySoundR(openSound);
    isDragged = false;
    OnOpen();
  }
  //Opens the window
  inline void CloseWindow() noexcept {
    if (!isWindowOpen) return;
    isWindowOpen = false;
    PlaySoundR(closeSound);
    isDragged = false;
    OnClose();
  }
  //Switches from open to close and reverse
  inline void ToggleWindow() noexcept {
    if (!isWindowOpen) {
      OpenWindow();
    } else {
      CloseWindow();
    }
  }
  //Resets position to base
  inline void ResetPosition() noexcept {
    wholeWindow.x = basePosition.x;
    wholeWindow.y = basePosition.y;

    header_bar.x = basePosition.x;
    header_bar.y = basePosition.y + 2;
  }
  [[nodiscard]] inline bool IsInsideWindowBounds(float x, float y) const noexcept {
    return x >= wholeWindow.x && x < wholeWindow.x + wholeWindow.width &&
           y >= wholeWindow.y && y < wholeWindow.y + wholeWindow.height;
  }
  //Default way to draw the window
  void DrawWindow() const noexcept {
    RectangleR scaled_whole = SCALE_RECT(wholeWindow);
    RectangleR scaled_head = SCALE_RECT(header_bar);

    float ROUNDNESS = 0.1F;
    if (wholeWindow.width > 450) {
      ROUNDNESS = 0.05F;
    }

    DrawRectangleRounded(scaled_whole, ROUNDNESS, GAME_SETTINGS.roundSegments,
                         Colors::LightGrey);

    DrawRectangleRounded(scaled_head, 0.5F, GAME_SETTINGS.roundSegments,
                         isHeaderHovered ? isDragged ? Colors::mediumLightGreyDarker
                                                     : Colors::mediumLightGreyBitDarker
                                         : Colors::mediumLightGrey);

    DrawRectangleRoundedLines(scaled_head, 1.5F, GAME_SETTINGS.roundSegments, 2,
                              Colors::darkBackground);
    DrawRectangleRoundedLines(scaled_whole, ROUNDNESS, GAME_SETTINGS.roundSegments, 3,
                              Colors::darkBackground);
    Util::DrawCenteredText(
        ANT_PARTY, SCALE(fontSize), header_text, scaled_whole.x + scaled_whole.width / 2,
        scaled_whole.y + scaled_head.height / 4, Colors::darkBackground);
  }

  //Getters
 public:
  [[nodiscard]] inline Point GetWindowPos() const noexcept {
    return {wholeWindow.x, wholeWindow.y};
  }

  //Events methods
 public:
  virtual inline void OnOpen(){};
  virtual inline void OnClose(){};

#define WINDOW_LOGIC()                                     \
  if (IsKeyPressed(windowOpenKey)) {                       \
    if (isWindowOpen) {                                    \
      CloseWindow();                                       \
    } else {                                               \
      OpenWindow();                                        \
    }                                                      \
  }                                                        \
                                                           \
  if (!isWindowOpen) {                                     \
    return;                                                \
  }                                                        \
                                                           \
  if (isDragged && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) { \
    auto mouse_pos = MOUSE_POS;                            \
    auto delta_x = (mouse_pos.x - lastMousePos.x);         \
    auto delta_y = (mouse_pos.y - lastMousePos.y);         \
    wholeWindow.x += delta_x;                              \
    wholeWindow.y += delta_y;                              \
    header_bar.x += delta_x;                               \
    header_bar.y += delta_y;                               \
    lastMousePos = mouse_pos;                              \
  } else {                                                 \
    isDragged = false;                                     \
  }

#define WINDOW_UPDATE()                                                               \
  if (!isWindowOpen) {                                                                \
    return;                                                                           \
  }                                                                                   \
  if (WINDOW_FOCUSED) return;                                                         \
  isHeaderHovered = false;                                                            \
  if (!WINDOW_FOCUSED && CheckCollisionPointRec(MOUSE_POS, SCALE_RECT(header_bar)) && \
      !DRAGGED_ITEM) {                                                                \
    isHeaderHovered = true;                                                           \
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {                                       \
      if (!isDragged) {                                                               \
        isDragged = true;                                                             \
        lastMousePos = MOUSE_POS;                                                     \
      }                                                                               \
    }                                                                                 \
  }                                                                                   \
                                                                                      \
  if (!WINDOW_FOCUSED) {                                                              \
    WINDOW_FOCUSED =                                                                  \
        isDragged || CheckCollisionPointRec(MOUSE_POS, SCALE_RECT(wholeWindow));      \
  }
};
#endif  //MAGEQUEST_SRC_UI_WINDOW_H_
