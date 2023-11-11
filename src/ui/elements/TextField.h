#ifndef MAGEQUEST_SRC_UI_ELEMENTS_TEXTFIELD_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_TEXTFIELD_H_

struct TextField {
  inline static constexpr int BLINK_DELAY = 30;  // Adjust this for blink rate
  // Textfield text
  std::string text;
  RectangleR hitBox;
  const char* preFix;
  int8_t cursorPos = 0;
  int8_t maxCharacters;
  int blinkCounter = 0;
  bool isFocused = false;
  bool showCursor = true;
  bool wasUpdated = false;
  TextField(float width, float height, int8_t maxCharacters, const char* preFix)
      : hitBox(0, 0, width, height), maxCharacters(maxCharacters), preFix(preFix) {}

 public:
  inline void Draw(float x, float y) noexcept {
    InternalImpl(x, y);
    DrawTextField();
  }
  //Returns true if there was an update and consumes it
  inline bool WasUpdated() noexcept {
    bool ret = wasUpdated;
    wasUpdated = false;
    return ret;
  }
  //Runs the input logic for the textfield // consumes all inputs after it automatically
  inline void UpdateLogic() noexcept {
    if (isFocused) {
      HandleInput();
    }
  }
  inline void ClearText() noexcept {
    text.clear();
    cursorPos = 0;
  }
  inline void Reset() noexcept {
    ClearText();
    isFocused = false;
  }

 private:
  //Draws the main content
  inline void DrawTextField() noexcept {
    Util::DrawRightAlignedText(MINECRAFT_REGULAR, 16, preFix, hitBox.x - 3, hitBox.y,
                               Colors::darkBackground);
    DrawRectangleProFast(hitBox.x, hitBox.y, hitBox.width, hitBox.height,
                         isFocused ? Colors::darkBackground : Colors::mediumLightGrey);
    DrawRectangleLinesEx(hitBox, 1, Colors::black);
    std::string displayText = text;
    if (showCursor) {
      displayText.insert(cursorPos, "|");
    }
    DrawTextExR(MINECRAFT_REGULAR, displayText.c_str(), {hitBox.x + 1, hitBox.y}, 16,
                0.5F, WHITE);
  }
  //Handles input and the functions
  inline void HandleInput() {
    int charValue = GetCharPressed();
    if (charValue > 0 && text.size() < maxCharacters) {
      text.insert(cursorPos, 1, static_cast<char>(charValue));
      cursorPos++;
      wasUpdated = true;
    }

    if (IsKeyPressed(KEY_BACKSPACE) && cursorPos > 0) {
      text.erase(cursorPos - 1, 1);
      wasUpdated = true;
      cursorPos--;
    }

    if (IsKeyPressed(KEY_RIGHT) && cursorPos < text.length()) {
      cursorPos++;
    }
    if (IsKeyPressed(KEY_LEFT) && cursorPos > 0) {
      cursorPos--;
    }

    // Handle blinking cursor
    blinkCounter++;
    if (blinkCounter >= BLINK_DELAY) {
      showCursor = !showCursor;
      blinkCounter = 0;
    }
    ConsumeKeyPresses();
  }
  //Updates the internal state
  inline void InternalImpl(float x, float y) noexcept {
    hitBox.x = x;
    hitBox.y = y;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      isFocused = CheckCollisionPointRec(MOUSE_POS, hitBox);
      blinkCounter = 0;
    }
    showCursor = isFocused;

    if (!WINDOW_FOCUSED) {
      WINDOW_FOCUSED = isFocused;
    }
  }
};

#endif  //MAGEQUEST_SRC_UI_ELEMENTS_TEXTFIELD_H_
