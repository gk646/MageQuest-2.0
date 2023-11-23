#ifndef MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
#define MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
struct ItemSlot;
namespace Util {
//Returns a value between 0-1 based on their alphabetical distance // handles capitalization
inline static float GetCharacterSimilarity(char base, char check) noexcept {
  if (base == check) return 1.0F;
  base -= base > 96 ? 97 : 65;
  check -= check > 96 ? 97 : 65;
  auto diff = (float)std::abs(base - check);
  diff = std::max(0.0F, std::min(26.0F, diff));
  return 1.0F - (1.0F / 26.0F) * diff;
}
//Returns a value between 0-1 on how similar the strings are based on their alphabetical distance
//Only checks until either one ends and doesn't penalize unequal length -> bla and blabbbbbb = 1.0F
inline static float GetStringSimilarityPrefix(const std::string& base,
                                              const std::string& check) noexcept {
  if (base.empty() || check.empty()) return 0.0F;
  float sum = 0;
  int i = 0;
  while (i < base.size() && i < check.size()) {
    sum += GetCharacterSimilarity(base[i], check[i]);
    i++;
  }
  while (i < base.size()) {
    i++;
  }
  return sum / i;
}
//Returns true if the given array contains the given value
template <typename T>
inline static bool ArrayContains(const T* arr, const T& val, int size) noexcept {
  for (int i = 0; i < size; i++) {
    if (arr[i] == val) {
      return true;
    }
  }
  return false;
}
//creates a 2D array of the given type and size
template <typename T>
inline static T** Create2DArr(int x, int y) {
  auto ptr = new T*[x];
  for (int i = 0; i < y; i++) {
    ptr[i] = new T[y];
  }
  return ptr;
}
//frees a 2D array of the given type and size
template <typename T>
inline static void Delete2DArr(T** arr, int rows) {
  for (int i = 0; i < rows; i++) {
    delete[] arr[i];
  }
  delete[] arr;
}
//Draws text aligned to the right with "align" / uses 0.5 spacing
inline static void DrawRightAlignedText(const Font& font, float fontSize, const char* txt,
                                        float align, float y, const Color& color) {
  DrawTextExR(font, txt, {align - MeasureTextEx(font, txt, fontSize, 0.5F).x, y},
              fontSize, 0.5F, color);
}
//Draws text centered to "align" / uses 0.5 spacing
inline static void DrawCenteredText(const Font& font, float fontSize, const char* txt,
                                    float align, float y, const Color& color) {
  DrawTextExR(font, txt, {align - MeasureTextEx(font, txt, fontSize, 0.5F).x / 2, y},
              fontSize, 0.5F, color);
}
//Loads individual lines from a file into a vector
static std::vector<std::string> LoadTextLines(const std::string& filePath) {
  std::vector<std::string> lines;
  std::ifstream file(filePath);

  if (!file.is_open()) {
    return lines;
  }

  std::string line;
  while (getline(file, line)) {
    lines.push_back(line);
  }

  file.close();
  return lines;
}
//Draws outlined text by doing multiple draw calls with offset and coloring
inline static void DrawOutlineText(const Font& font, float fontSize, const char* txt,
                                   float dx, float dy, int thickness,
                                   const Color& textColor,
                                   const Color& outlineColor) noexcept {
  for (int x = -thickness; x <= thickness; x++) {
    for (int y = -thickness; y <= thickness; y++) {
      if (x != 0 || y != 0) {
        DrawTextExR(font, txt, {dx + x, dy + y}, fontSize, 0.8F, outlineColor);
      }
    }
  }

  DrawTextExR(font, txt, {dx, dy}, fontSize, 0.8F, textColor);
}
//Wraps "txt" according to "width" / Uses 0.5 spacing / "lineBreaks" is set the number of line breaks
inline static std::string WrapText(const std::string& txt, float width, const Font& font,
                                   float fontSize, int* lineBreaks = nullptr) {
  if (lineBreaks) {
    *lineBreaks = 0;
  }

  std::istringstream iss(txt);
  std::string word;
  std::string wrappedText;

  wrappedText.reserve(txt.size() + 50);

  float currentLineWidth = 0;
  float spaceWidth = MeasureTextEx(font, " ", fontSize, 0.5).x;

  while (iss >> word) {
    float wordWidth = MeasureTextEx(font, word.c_str(), fontSize, 0.5).x;
    if (currentLineWidth + wordWidth + spaceWidth > width) {
      wrappedText += '\n';
      if (lineBreaks) {
        (*lineBreaks)++;
      }
      currentLineWidth = 0;
    }
    wrappedText += word + " ";
    currentLineWidth += wordWidth + spaceWidth;
  }
  return wrappedText;
}
template <typename T>
inline static int GetMaxValue(const T* arr, int size) noexcept {
  int max = INT_MIN;
  for (uint_fast32_t i = 0; i < size; i++) {
    if (arr[i] > max) max = arr[i];
  }
  return max;
}
inline static void DrawWrappedText(const Vector2& pos, const std::string& txt,
                                   float width, const Font& font, float fontSize,
                                   Color tint) {
  DrawTextExR(font, WrapText(txt, width, font, fontSize).c_str(), pos, fontSize, 0.5F,
              tint);
}
//Splits a string around "delim"
inline static std::vector<std::string> SplitString(const std::string& s,
                                                   char delim) noexcept {
  std::vector<std::string> result;
  std::string_view sv(s);
  while (!sv.empty()) {
    size_t pos = sv.find(delim);
    if (pos == std::string_view::npos) {
      result.emplace_back(sv);
      break;
    } else {
      result.emplace_back(sv.substr(0, pos));
      sv.remove_prefix(pos + 1);
    }
  }
  return result;
}
//Loads .mgi (simple table format) into a vector
std::vector<std::vector<std::string>> ReadMGI(const std::string& filePath) noexcept {
  auto ret = std::vector<std::vector<std::string>>{};
  std::ifstream file(ASSET_PATH + filePath);

  if (!file.is_open()) {
    std::cerr << "Failed to open .mgi (MageQuestInfo) file:" << filePath << std::endl;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.starts_with('#')) continue;
    if (line.empty()) break;
    ret.push_back(std::move(SplitString(line, '|')));
  }
  return ret;
}
//Splits the given string by "," and takes the first two values with "std::stoi" into the returned PointI
inline static PointI ParsePointI(const std::string& string) noexcept {
  return {std::stoi(SplitString(string, ',')[0]), std::stoi(SplitString(string, ',')[1])};
}
inline static Point ParsePoint(const std::string& string) noexcept {
  return {std::stof(SplitString(string, ',')[0]), std::stof(SplitString(string, ',')[1])};
}
//Creates a new string for the skill tooltips formatted with numbers
inline static std::string CreateToolTipString(const std::string& s, float damage,
                                              float val1 = FLT_MAX,
                                              float val2 = FLT_MAX) {
  std::string ret = s;
  size_t pos;

  pos = ret.find("MAX_DMG");
  if (pos != std::string::npos) {
    ret.replace(pos, 7, std::to_string((int)std::round(damage * 1.1F)));
  }

  pos = ret.find("MIN_DMG");
  if (pos != std::string::npos) {
    ret.replace(pos, 7, std::to_string((int)std::round(damage * 0.9F)));
  }

  if (val1 != FLT_MAX) {
    pos = ret.find("VAL1");
    if (pos != std::string::npos) {
      ret.replace(pos, 4, std::to_string((int)val1));
    }
  }

  if (val2 != FLT_MAX) {
    pos = ret.find("VAL2");
    if (pos != std::string::npos) {
      ret.replace(pos, 4, std::to_string((int)val2));
    }
  }

  return ret;
}
//Creates a new string for the effect tooltips formatted with numbers
inline static std::string CreateEffectToolTipString(const std::string& s, float duration,
                                                    float amount = FLT_MAX,
                                                    float val3 = FLT_MAX,
                                                    const char* stringVal = nullptr) {
  std::string ret = s;
  size_t pos;
  char buffer[4];

  if (duration != FLT_MAX) {
    pos = ret.find("DURATION");
    if (pos != std::string::npos) {
      ret.replace(pos, 8, std::to_string((int)(duration / 60.0F)));
    }
  }

  if (amount != FLT_MAX) {
    pos = ret.find("AMOUNT");
    if (pos != std::string::npos) {
      snprintf(buffer, sizeof(buffer), "%.1f", amount);
      ret.replace(pos, 6, buffer);
    }
  }

  if (val3 != FLT_MAX) {
    pos = ret.find("VAL3");
    if (pos != std::string::npos) {
      snprintf(buffer, sizeof(buffer), "%.1f", val3);
      ret.replace(pos, 4, buffer);
    }
  }

  if (stringVal) {
    pos = ret.find("STRING");
    if (pos != std::string::npos) {
      ret.replace(pos, 6, stringVal);
    }
  }

  return ret;
}
//Draws a swiping rectangular cooldown animation by drawing lines from the center to the bound points of the square
inline static void DrawSwipeCooldownEffect(float x, float y, float size, int totalTicks,
                                           int currentTicks) noexcept {
  if (currentTicks < totalTicks) {
    float side1, side2, side3, side4, side5;
    float coolDownCoefficient;
    coolDownCoefficient = currentTicks * (size * 4 / totalTicks);
    side1 = size / 2;
    side2 = 0;
    side3 = 0;
    side4 = 0;
    side5 = 0;
    if (coolDownCoefficient > 0) side1 += coolDownCoefficient;
    if (coolDownCoefficient > size / 2) side2 += coolDownCoefficient - size / 2;
    if (coolDownCoefficient > size * 1.5F) side3 += coolDownCoefficient - size * 1.5F;
    if (coolDownCoefficient > size * 2.5F) side4 += coolDownCoefficient - size * 2.5F;
    if (coolDownCoefficient > size * 3.5F) side5 += coolDownCoefficient - size * 3.5F;

    for (int i = side1; i <= size; i++) {
      DrawLine(x + size / 2, y + size / 2, x + i, y, Colors::mediumLightGreyTransparent);
    }
    for (int i = side2; i <= size; i++) {
      DrawLine(x + size / 2, y + size / 2, x + size, y + i,
               Colors::mediumLightGreyTransparent);
    }
    for (int i = side3; i <= size; i++) {
      DrawLine(x + size / 2, y + size / 2, x + size - i, y + size,
               Colors::mediumLightGreyTransparent);
    }
    for (int i = side4; i <= size; i++) {
      DrawLine(x + size / 2, y + size / 2, x, y + size - i,
               Colors::mediumLightGreyTransparent);
    }
    for (int i = side5; i <= size / 2; i++) {
      DrawLine(x + size / 2, y + size / 2, x + i, y, Colors::mediumLightGreyTransparent);
    }
  }
}
//Returns a correctly aligned rect inside the window bounds with the given measures
inline static RectangleR GetToolTipRect(float width, float height) noexcept {
  RectangleR ret = {0, 0, width, height};
  auto mouse = GetMousePosition();
  if (mouse.x - width < 0) {
    ret.x = mouse.x + 10;
  } else {
    ret.x = mouse.x - width;
  }
  if (mouse.y - height < 0) {
    ret.y = mouse.y + 5;
  } else {
    ret.y = mouse.y - height;
  }
  return ret;
}
inline static void SelectionSortInventorySlot(ItemSlot* arr, uint_32_cx len,
                                              bool ascending = true);
//Key press module / used for detecting key press on update tick rate(60)
inline static bool e_previous[2] = {false, false};
inline static void Update() noexcept {
  e_previous[0] = e_previous[1];
  e_previous[1] = IsKeyDown(KEY_E);
}
inline static bool EPressed() noexcept {
  return e_previous[0] && !e_previous[1];
}
}  // namespace Util
#endif  //MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
