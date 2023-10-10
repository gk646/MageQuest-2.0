#ifndef MAGE_QUEST_SRC_LOADING_LOADINGUTIL_H_
#define MAGE_QUEST_SRC_LOADING_LOADINGUTIL_H_

namespace load_util {
inline const std::array<float, 13> weights{2, 2, 1, 5, 10, 10, 10, 7, 1, 3, 2, 3, 3};
inline int cpu_steps = 7;
inline int current_step = 0;
}  // namespace load_util

void LoadStep(const std::function<void()>& func) {
  func();

  float total_weight = 0;
  for (auto num : load_util::weights) {
    total_weight += num;
  }
  float step_contribution =
      (load_util::weights[load_util::current_step] / total_weight) * 100;
  LoadingScreen::progress += step_contribution;
  load_util::current_step++;
}

#endif  //MAGE_QUEST_SRC_LOADING_LOADINGUTIL_H_
