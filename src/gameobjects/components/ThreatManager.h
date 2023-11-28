#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ELEMENTS_THREATMANAGER_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ELEMENTS_THREATMANAGER_H_
struct ThreatEntity {
  Entity* entity = nullptr;
  float threat = 0;
};

struct ThreatManager {
  static constexpr float THREAT_DROP = 0.2F / 60;
  std::array<ThreatEntity, 4> targets{};
  Monster* self;
  int8_t targetCount = 0;

  explicit ThreatManager(Monster* self) : self(self) {}
  ThreatManager& operator=(const ThreatManager& other) {
    if (this != &other) {
      targets = other.targets;
      targetCount = other.targetCount;
      self = other.self;
    }
    return *this;
  }
  inline void ResetEntity(Entity* ptr) noexcept {
    for (auto& te : targets) {
      if(te.entity == ptr){
        te.entity = nullptr;
        te.threat = 0;
      }
    }
    targetCount = 0;
  }
  inline void Reset() noexcept {
    for (auto& te : targets) {
      te.entity = nullptr;
      te.threat = 0;
    }
    targetCount = 0;
  }
  inline void Update() noexcept;
  [[nodiscard]] inline Entity* GetHighestThreatTarget() noexcept {
    float HighestThreat = 0;
    Entity* HighestPtr = nullptr;
    for (auto& te : targets) {
      if (te.entity) {
        if (te.threat > HighestThreat) {
          HighestThreat = te.threat;
          HighestPtr = te.entity;
        }
      }
    }
    return HighestPtr;
  }
  inline void AddThreat(const Entity* ent, float threat) noexcept {
    for (auto& te : targets) {
      if (te.entity == ent) {
        te.threat += threat;
      }
    }
  }

 private:
  inline void AddTarget(Entity* NewEnt, const float threat) noexcept {
    for (auto& te : targets) {
      if (!te.entity) {
        te.entity = NewEnt;
        te.threat = threat;
        targetCount++;
        break;
      }
    }
  }
  inline void RemoveTarget(Entity* NewEnt) noexcept {
    for (auto& te : targets) {
      if (te.entity == NewEnt) {
        te.entity = nullptr;
        te.threat = 0;
        targetCount--;
        break;
      }
    }
  }
};

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ELEMENTS_THREATMANAGER_H_
