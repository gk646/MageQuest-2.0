#ifndef MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERABILITIESUTIL_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERABILITIESUTIL_H_
namespace MonsterUtil {
//Clones the given baseIndicator "amount" times around a circle line with the given measures
static void SpawnAoeCircular(const Point& pos, int distanceFromCenter, int amount,
                             AoEIndicator* baseIndicator) {
  if (!baseIndicator || amount <= 0) return;

  // Generate a random starting angle in radians
  float startingAngle = RANGE_01(RNG_ENGINE) * 2 * PI;

  // Angle increment for each subsequent indicator
  float angleIncrement = 2 * PI / amount;

  for (int i = 0; i < amount; i++) {
    // Calculate the angle for this indicator
    float angle = startingAngle + i * angleIncrement;

    // Calculate the position for this indicator
    float x = pos.x() + distanceFromCenter * cos(angle);
    float y = pos.y() + distanceFromCenter * sin(angle);

    // Create a new indicator and set its position
    auto newIndicator = new AoEIndicator(*baseIndicator);
    newIndicator->pos = Point{x, y};

    // Add the new indicator to world objects (assuming WORLD_OBJECTS is a container)
    WORLD_OBJECTS.emplace_back(newIndicator);
  }

  // Clean up the base indicator
  delete baseIndicator;
}
}  // namespace MonsterUtil
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_MONSTERS_MONSTERABILITIESUTIL_H_
