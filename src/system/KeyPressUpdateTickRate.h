#ifndef MAGEQUEST_SRC_SYSTEM_KEYPRESSUPDATETICKRATE_H_
#define MAGEQUEST_SRC_SYSTEM_KEYPRESSUPDATETICKRATE_H_
namespace IsKeyPressedU {
inline static bool e_previous[2] = {false, false};
inline static void update() noexcept {
  e_previous[0] = e_previous[1];
  e_previous[1] = IsKeyDown(KEY_E);
}
inline static bool E() noexcept {
  return e_previous[0] && !e_previous[1];
}
}  // namespace IsKeyPressedU
#endif  //MAGEQUEST_SRC_SYSTEM_KEYPRESSUPDATETICKRATE_H_
