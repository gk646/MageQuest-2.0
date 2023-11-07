#ifndef MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
#define MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
namespace Util {
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
  for (uint_fast32_t i = 0; i < y; i++) {
    ptr[i] = new T[y];
  }
  return ptr;
}
//frees a 2D array of the given type and size
template <typename T>
inline static void Delete2DArr(T** arr, int rows) {
  for (uint_fast32_t i = 0; i < rows; i++) {
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
inline static std::string CreateToolTipString(const std::string& s, float damage,
                                              float val1 = 0.0f, float val2 = 0.0f) {
  std::string ret = s;
  std::string minDmgStr = std::to_string((int)(damage * 0.9F));
  std::string maxDmgStr = std::to_string((int)(damage * 1.1F));

  std::string val1Str = std::to_string((int)val1);
  std::string val2Str = std::to_string((int)val2);

  size_t pos;

  pos = s.find("MAX_DMG");
  if (pos != std::string::npos) {
    ret.replace(pos, 7, maxDmgStr);
  }

  pos = s.find("MIN_DMG");
  if (pos != std::string::npos) {
    ret.replace(pos, 7, minDmgStr);
  }

  if (val1 != 0) {
    pos = s.find("VAL1");
    if (pos != std::string::npos) {
      ret.replace(pos, 4, val1Str);
    }
  }

  if (val2 != 0) {
    pos = s.find("VAL2");
    if (pos != std::string::npos) {
      ret.replace(pos, 4, val2Str);
    }
  }

  return ret;
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
