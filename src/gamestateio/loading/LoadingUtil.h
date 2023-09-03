#ifndef MAGE_QUEST_SRC_LOADING_LOADINGUTIL_H_
#define MAGE_QUEST_SRC_LOADING_LOADINGUTIL_H_

namespace load_util {
inline const std::array<float, 8> weights{4, 1, 10, 15, 1, 5, 1, 2};
inline int cpu_steps = 4;
inline int current_step = 0;
}  // namespace load_util

void load_step(const std::function<void()>& func) {
  func();

  float total_weight = 0;
  for (auto num : load_util::weights) {
    total_weight += num;
  }
  float step_contribution = (load_util::weights[load_util::current_step] / total_weight) * 100;
  LoadingScreen::progress += step_contribution;
  load_util::current_step++;
}
void load_window_icon() {
  Image icon = LoadImage((ASSET_PATH + "dnd_logo.png").c_str());
  SetWindowIcon(icon);
  UnloadImage(icon);
}

#endif  //MAGE_QUEST_SRC_LOADING_LOADINGUTIL_H_
