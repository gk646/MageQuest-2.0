#ifndef MAGEQUEST_SRC_GRAPHICS_DIALOGUERENDER_H_
#define MAGEQUEST_SRC_GRAPHICS_DIALOGUERENDER_H_

namespace TextRenderer {
inline static constexpr int BASE_DIALOGUE_BOX_WIDTH = 365;
inline static constexpr int BASE_DIALOGUE_BOX_HEIGHT = 120;
inline static int spriteCounter = 0;
inline static std::string* playerText = nullptr;
inline static float* playerDialogueCount = nullptr;
inline static void DrawContinueButton(float startX, float width, float startY,
                                      float height) noexcept {
  int num = spriteCounter % 160 / 40;
  switch (num) {
    case 0:
      DrawTextureScaled(textures::ui::dialogContinue,
                        {startX + width - 20, startY + height - 35, 16, 16}, 0, false, 0,
                        WHITE);
      break;
    case 1:
      DrawTextureScaled(textures::ui::dialogContinue2,
                        {startX + width - 20, startY + height - 35, 16, 16}, 0, false, 0,
                        WHITE);
      break;
    case 2:
      DrawTextureScaled(textures::ui::dialogContinue3,
                        {startX + width - 20, startY + height - 35, 16, 16}, 0, false, 0,
                        WHITE);
      break;
    case 3:
      DrawTextureScaled(textures::ui::dialogContinue2,
                        {startX + width - 20, startY + height - 35, 16, 16}, 0, false, 0,
                        WHITE);
      break;
  }
  spriteCounter++;
}
//TODO fix dialogue choice dissaper // make smooother // fix dialogue hide counter to be more dynamic
static void RenderDialogue(int x, int y, const std::string* text, float& count,
                           bool last) noexcept {
  if (!text || count == 0.0F) return;
  float width = BASE_DIALOGUE_BOX_WIDTH;
  float height = BASE_DIALOGUE_BOX_HEIGHT;

  float startX = x - width / 2;
  float startY = y - height;

  DrawRectangleRounded({startX, startY, width, height - 10}, 0.2F,
                       GAME_SETTINGS.roundSegments, Colors::black_transparent);
  DrawRectangleRoundedLines({startX, startY, width, height - 10}, 0.2F,
                            GAME_SETTINGS.roundSegments, 2, Colors::white);
  if (count > text->size()) {
    count = 1000.0F;
    if (!last) {
      DrawContinueButton(startX, width, startY, height);
    }
  } else {
    if (!IsSoundPlaying(sound::speak)) {
      PlaySoundR(sound::speak);
    }
  }

  auto wrappedText =
      Util::WrapText(text->substr(0, count), width - 3, MINECRAFT_REGULAR, 17);
  DrawTextExR(MINECRAFT_REGULAR, wrappedText.c_str(), {startX + 3, startY + 3}, 17, 0.5,
              WHITE);
}
static void RenderPlayerThought() noexcept {
  if (!playerText) return;
  *playerDialogueCount += 0.4F * (60.0F / GetFPS());
  float width = BASE_DIALOGUE_BOX_WIDTH;
  float height = BASE_DIALOGUE_BOX_HEIGHT;

  float startX = CAMERA_X - width / 2;
  float startY = CAMERA_Y - height;

  if (*playerDialogueCount < (float)playerText->size()) {
    if (!IsSoundPlaying(sound::speak)) {
      PlaySoundR(sound::speak);
    }
  }
  if (*playerDialogueCount > 300) {
    playerText = nullptr;
    playerDialogueCount = nullptr;
    return;
  }

  auto wrappedText = Util::WrapText(playerText->substr(0, (int)*playerDialogueCount),
                                    width - 3, MINECRAFT_ITALIC, 17);
  DrawTextExR(MINECRAFT_ITALIC, wrappedText.c_str(), {startX + 3, startY + 3}, 17, 0.5,
              WHITE);
}
}  // namespace TextRenderer
#endif  //MAGEQUEST_SRC_GRAPHICS_DIALOGUERENDER_H_
