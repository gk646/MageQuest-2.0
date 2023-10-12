#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ELEMENTS_THREATMANAGER_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ELEMENTS_THREATMANAGER_H_
struct ThreatEntity{
  Entity* entity = nullptr;
  float threat = 0;
};

struct ThreatManager {
  static constexpr float THREAT_DROP = 0.2F / 60;
  std::array<ThreatEntity, 4> targets{};
  Monster& Self;
  int8_t TargetCount = 0;
  explicit ThreatManager(Monster& Self) : Self(Self) {}
  inline void Reset() noexcept {
    for (auto& te : targets) {
      te.entity = nullptr;
      te.threat = 0;
    }
    TargetCount = 0;
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
  inline void AddTarget(Entity* NewEnt, float threat) noexcept {
    for (auto& te : targets) {
      if (!te.entity) {
        te.entity = NewEnt;
        te.threat = threat;
        TargetCount++;
        break;
      }
    }
  }
  inline void RemoveTarget(Entity* NewEnt) noexcept {
    for (auto& te : targets) {
      if (te.entity == NewEnt) {
        te.entity = nullptr;
        te.threat = 0;
        TargetCount--;
        break;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ELEMENTS_THREATMANAGER_H_
