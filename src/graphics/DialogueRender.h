#ifndef MAGEQUEST_SRC_GRAPHICS_DIALOGUERENDER_H_
#define MAGEQUEST_SRC_GRAPHICS_DIALOGUERENDER_H_

namespace DialogueRender {
inline static constexpr int base_width = 365;
inline static constexpr int base_height = 120;
void render_npc(int x, int y, const std::string* text, float& count) {
  float width = SCALE(base_width);
  float height = SCALE(base_height);

  float startX = x-width/2;
  float startY = y-height;

  DrawRectangleRounded({startX,startY,width, height-10},0.2F,ROUND_SEGMENTS,Colors::black_transparent);
  DrawRectangleRoundedLines({startX,startY,width, height-10},0.2F,ROUND_SEGMENTS,2,Colors::white);
  if (count > text->size()) {
    count = 1000;
  }

  std::string toRender = text->substr(0, count);

  std::istringstream iss(toRender);
  std::string word;
  std::string wrappedText;
  float currentLineWidth = 0;
  float spaceWidth = MeasureTextEx(MINECRAFT_REGULAR, " ", 17, 0.5).x;

  while (iss >> word) {
    float wordWidth = MeasureTextEx(MINECRAFT_REGULAR, word.c_str(), 17, 0.5).x;
    if (currentLineWidth + wordWidth + spaceWidth > width-3) {
      wrappedText += '\n';
      currentLineWidth = 0;
    }
    wrappedText += word + " ";
    currentLineWidth += wordWidth + spaceWidth;
  }

  DrawTextExR(MINECRAFT_REGULAR, wrappedText.c_str(), {startX+3, startY+3}, 17, 0.5, WHITE);
}


}  // namespace DialogueRender
#endif  //MAGEQUEST_SRC_GRAPHICS_DIALOGUERENDER_H_
