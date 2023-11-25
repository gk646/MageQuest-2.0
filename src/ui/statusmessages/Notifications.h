#ifndef MAGEQUEST_SRC_UI_NOTIFICATIONS_NOTIFICATIONS_H_
#define MAGEQUEST_SRC_UI_NOTIFICATIONS_NOTIFICATIONS_H_

#include "StatusText.h"

namespace Notifications {
inline static StatusMessageType status;
inline static bool isNegative = false;
inline static uint8_t upCounter = 0;
inline static std::string currentText;

static void PlayNotificationSound(StatusMessageType newStatus) noexcept {}
inline static void UpdateStatusMessage(std::string text, StatusMessageType newStatus,
                                       bool negative = true) noexcept {
  currentText = std::move(text);
  isNegative = negative;
  upCounter = 1;
  status = newStatus;
  PlayNotificationSound(newStatus);
}
inline static void DrawStatusText() {
  if (upCounter > 0) {
    Util::DrawCenteredText(ANT_PARTY, 16, currentText.c_str(), SCREEN_WIDTH / 2,
                           SCREEN_HEIGHT * 0.2F,
                           isNegative ? Colors::Red : Colors::green_npc);
    upCounter++;
  }
}
inline static void Draw() noexcept {
  DrawStatusText();
  StatusText::Draw();
}
inline static void Update() noexcept {
  StatusText::Update();
}
}  // namespace Notifications
#endif  //MAGEQUEST_SRC_UI_NOTIFICATIONS_NOTIFICATIONS_H_
