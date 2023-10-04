#ifndef MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
#define MAGE_QUEST_SRC_UTIL_MATHUTIL_H_

template <typename T>
T** get_2D_array(int x, int y) {
  auto ptr = new T*[x];
  for (uint_fast32_t i = 0; i < y; i++) {
    ptr[i] = new T[y];
  }
  return ptr;
}
template <typename T>
void delete_2D_array(T** arr, int rows) {
  for (uint_fast32_t i = 0; i < rows; i++) {
    delete[] arr[i];
  }
  delete[] arr;
}

inline void transcribe_string_into_uint8(int offset, const std::string& s, uint8_t* arr) {
  for (uint_fast32_t i = 0; i < s.size(); i++) {
    arr[i + offset] = (uint8_t)s[i];
  }
  arr[s.size() + offset] = '\0';
}
std::string read_string_from_uint8(uint8_t* arr, int offset) {
  return {reinterpret_cast<char*>(arr + offset)};
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
                                   float dx, float dy, int thickness,const Color& textColor,
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

std::vector<std::string> split(const std::string& s, char delim) noexcept {
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
inline PointI ParsePointI(const std::string& string) noexcept {
  return {std::stoi(split(string, ',')[0]), std::stoi(split(string, ',')[1])};
}
inline Point ParsePoint(const std::string& string) noexcept {
  return {std::stof(split(string, ',')[0]), std::stof(split(string, ',')[1])};
}

namespace UpdateTickFunctions {
inline static bool e_previous[2] = {false, false};
inline static void update() noexcept {
  e_previous[0] = e_previous[1];
  e_previous[1] = IsKeyDown(KEY_E);
}
inline static bool EPressed() noexcept {
  return e_previous[0] && !e_previous[1];
}
}  // namespace UpdateTickFunctions
#endif  //MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
