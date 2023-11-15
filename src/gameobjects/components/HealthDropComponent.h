#ifndef MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_HEALTHDROPCOMPONENT_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_HEALTHDROPCOMPONENT_H_

//A simple component setting the passed bool to true if a certain health threshold is met
struct HealthDropComponent {
  std::array<float, 4> thresholds;
  std::array<bool, 4> triggered;

  HealthDropComponent(float p1, float p2, float p3, float p4)
      : thresholds({p1, p2, p3, p4}), triggered({false, false, false, false}) {}

  inline void Update(bool& val, const EntityStats& stats) noexcept {
    float maxHealth = stats.effects[MAX_HEALTH];
    float health = stats.health;

    for (size_t i = 0; i < thresholds.size(); ++i) {
      float lowerBound = (i < thresholds.size() - 1) ? thresholds[i + 1] : 0.0f;
      if (!triggered[i] && health <= maxHealth * thresholds[i] && health > maxHealth * lowerBound) {
        val = true;
        triggered[i] = true;
        break;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_COMPONENTS_HEALTHDROPCOMPONENT_H_
