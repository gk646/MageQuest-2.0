#ifndef MAGEQUEST_SRC_GRAPHICS_WORLDANIMATIONS_H_
#define MAGEQUEST_SRC_GRAPHICS_WORLDANIMATIONS_H_
#include "elements/AnimationList.h"

namespace WorldAnimations {
AnimationList bigFish =
    AnimationList({1118, 1119, 1120, 1121, 1122, 1123, 1124, 1125}, 8, 20);
AnimationList smallFish =
    AnimationList({1131, 1132, 1133, 1134, 1135, 1136, 1137, 1138}, 8, 15);
AnimationList lines3 =
    AnimationList({1144, 1145, 1146, 1147, 1148, 1149, 1150, 1151}, 8, 20);
AnimationList lines2 =
    AnimationList({1157, 1158, 1159, 1160, 1161, 1162, 1163, 1164}, 8, 20);
AnimationList lines1 =
    AnimationList({1170, 1171, 1172, 1173, 1174, 1175, 1176, 1177}, 8, 15);
AnimationList water =
    AnimationList({911, 1262, 1263, 1264, 1265, 1266, 1267, 1268}, 8, 15);
AnimationList wall_torch =
    AnimationList({1404, 1405, 1406, 1407, 1408, 1409, 1410, 1411}, 8, 8);
AnimationList campfire = AnimationList({1528, 1529, 1530, 1531}, 4, 15);
AnimationList flowersred = AnimationList({1235, 1240, 1245, 2140}, 4, 25);
AnimationList flowersred1 = AnimationList({1236, 1241, 1246, 2141}, 4, 25);
AnimationList flowersred2 = AnimationList({1237, 1242, 1247, 2142}, 4, 25);
AnimationList flowersred3 = AnimationList({1238, 1243, 2138, 2143}, 4, 25);
AnimationList flowersred4 = AnimationList({1239, 1244, 2139, 2144}, 4, 25);

AnimationList whiteflower1 = AnimationList({1248, 1253, 1258, 2153}, 4, 25);
AnimationList whiteflower2 = AnimationList({1249, 1254, 1259, 2154}, 4, 25);
AnimationList whiteflower3 = AnimationList({1250, 1255, 1260, 2155}, 4, 25);
AnimationList whiteflower4 = AnimationList({1251, 1256, 2151, 2156}, 4, 25);
AnimationList whiteflower5 = AnimationList({1252, 1257, 2152, 2157}, 4, 25);

AnimationList lavaCasket = AnimationList({95, 96, 97}, 3, 35);
AnimationList lavaBecken = AnimationList({108, 109, 110}, 3, 35);
AnimationList waterCasket = AnimationList({121, 122, 123}, 3, 35);
AnimationList waterBecken = AnimationList({134, 135, 136}, 3, 35);
AnimationList spikes = AnimationList({222, 223, 224, 225}, 4, 35);

std::vector<AnimationList> animationLists{
    bigFish,     smallFish,    lines3,       lines2,       lines1,       water,
    wall_torch,  campfire,     flowersred,   flowersred1,  flowersred2,  flowersred3,
    flowersred4, whiteflower1, whiteflower2, whiteflower3, whiteflower4, whiteflower5,
    lavaCasket,  lavaBecken,   waterCasket,  waterBecken,  spikes};

inline static void CacheAnimationTiles() noexcept {
  for (auto& list : animationLists) {
    list.animatedTiles.clear();
  }
  for (uint_fast32_t l = 0; l < 3; l++) {
    auto layer = MapLayer(l);
    for (int i = 0; i < CURRENT_MAP_SIZE; i++) {
      for (int j = 0; j < CURRENT_MAP_SIZE; j++) {
        for (auto& animList : animationLists) {
          switch (layer) {
            case MapLayer::BACK_GROUND:
              if (animList.Contains(CURRENT_BACK_GROUND[i][j])) {
                animList.animatedTiles.emplace_back(PointI{i, j}, 0, layer, 0);
              }
              break;
            case MapLayer::MIDDLE_GROUND:
              if (animList.Contains(CURRENT_MIDDLE_GROUND[i][j])) {
                animList.animatedTiles.emplace_back(PointI{i, j}, 0, layer, 0);
              }
              break;
            case MapLayer::FRONT:
              if (animList.Contains(CURRENT_FORE_GROUND[i][j])) {
                animList.animatedTiles.emplace_back(PointI{i, j}, 0, layer, 0);
              }
              break;
          }
        }
      }
    }
  }
}
inline static void ProgressAnimations() noexcept {
  for (auto& list : animationLists) {
    list.ProgressTiles();
  }
}
}  // namespace WorldAnimations
#endif  //MAGEQUEST_SRC_GRAPHICS_WORLDANIMATIONS_H_
