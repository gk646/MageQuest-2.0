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

#endif  //MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
