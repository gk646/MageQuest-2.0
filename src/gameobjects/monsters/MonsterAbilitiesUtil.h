#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERABILITIESUTIL_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERABILITIESUTIL_H_
namespace MonsterUtil {

//Clones the given baseIndicator "amount" times around a circle line with the given measures
static void SpawnAoeCircular(const Point& pos, int distanceFromCenter, int amount,
                             AoEIndicator* baseIndicator) {
  if (!baseIndicator || amount <= 0) return;

  float startingAngle = RANGE_01(RNG_ENGINE) * 2 * PI;

  float angleIncrement = 2 * PI / amount;

  for (int i = 0; i < amount; i++) {
    float angle = startingAngle + i * angleIncrement;

    float x = pos.x() + distanceFromCenter * cos(angle);
    float y = pos.y() + distanceFromCenter * sin(angle);

    auto newIndicator = new AoEIndicator(*baseIndicator);
    newIndicator->pos = Point{x, y};

    WORLD_OBJECTS.emplace_back(newIndicator);
  }

  delete baseIndicator;
}
}  // namespace MonsterUtil
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERABILITIESUTIL_H_
