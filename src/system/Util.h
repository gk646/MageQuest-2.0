#ifndef MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
#define MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
namespace Util {
template <typename T>
T** Create2DArr(int x, int y) {
  auto ptr = new T*[x];
  for (uint_fast32_t i = 0; i < y; i++) {
    ptr[i] = new T[y];
  }
  return ptr;
}
template <typename T>
void Delete2DArr(T** arr, int rows) {
  for (uint_fast32_t i = 0; i < rows; i++) {
    delete[] arr[i];
  }
  delete[] arr;
}
inline static void DrawRightAlignedText(const Font& font, float fontSize, const char* txt,
                                        float align, float y, const Color& color) {
  DrawTextExR(font, txt, {align - MeasureTextEx(font, txt, fontSize, 0.5F).x, y},
              fontSize, 0.5F, color);
}
inline static void DrawCenteredText(const Font& font, float fontSize, const char* txt,
                                    float align, float y, const Color& color) {
  DrawTextExR(font, txt, {align - MeasureTextEx(font, txt, fontSize, 1).x / 2, y},
              fontSize, 1, color);
}
std::vector<std::string> loadStringsFromFile(const std::string& filePath) {
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
inline static void DrawOutlineText(const Font& font, float fontSize, const char* txt,
                                   float dx, float dy, int thickness,
                                   const Color& textColor,
                                   const Color& outlineColor) noexcept {
  for (int x = -thickness; x <= thickness; x++) {
    for (int y = -thickness; y <= thickness; y++) {
      if (x != 0 || y != 0) {
        DrawTextExR(font, txt, {dx + x, dy + y}, fontSize, 0.5F, outlineColor);
      }
    }
  }

  DrawTextExR(font, txt, {dx, dy}, fontSize, 0.5F, textColor);
}
std::string WrapText(const std::string& txt, float width, const Font& font,
                     float fontSize, int* lineBreakCount = nullptr) {
  if (lineBreakCount) {
    *lineBreakCount = 0;
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
      if (lineBreakCount) {
        (*lineBreakCount)++;
      }
      currentLineWidth = 0;
    }
    wrappedText += word + " ";
    currentLineWidth += wordWidth + spaceWidth;
  }
  return wrappedText;
}
std::vector<std::string> SplitString(const std::string& s, char delim) noexcept {
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
static std::vector<std::vector<std::string>> ReadMGI(
    const std::string& filePath) noexcept {
  auto ret = std::vector<std::vector<std::string>>{};
  std::ifstream file(ASSET_PATH + filePath);

  if (!file.is_open()) {
    std::cerr << "Failed to open .mgi (MageQuestInfo) file." << std::endl;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.starts_with('#')) continue;
    if (line.empty()) break;
    ret.push_back(std::move(SplitString(line, '|')));
  }
  return ret;
}
inline PointI ParsePointI(const std::string& string) noexcept {
  return {std::stoi(SplitString(string, ',')[0]), std::stoi(SplitString(string, ',')[1])};
}
inline Point ParsePoint(const std::string& string) noexcept {
  return {std::stof(SplitString(string, ',')[0]), std::stof(SplitString(string, ',')[1])};
}
inline std::string CreateToolTipString(const std::string& s, float damage,
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
inline static bool e_previous[2] = {false, false};
inline static void update() noexcept {
  e_previous[0] = e_previous[1];
  e_previous[1] = IsKeyDown(KEY_E);
}
inline static bool EPressed() noexcept {
  return e_previous[0] && !e_previous[1];
}
}  // namespace Util
#endif  //MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
