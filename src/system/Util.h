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
#endif  //MAGE_QUEST_SRC_UTIL_MATHUTIL_H_
