#ifndef MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
#define MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_

struct StatusEffect {
  inline static constexpr float TOOL_TIP_WIDTH = 180;
  inline static constexpr float TOOL_TIP_BASE_HEIGHT = 35;
  int16_t cadence;
  int16_t fullDuration;
  int16_t duration;
  int16_t stacks = 0;
  EffectType type;
  bool isDebuff;
  StatusEffect(bool isDebuff, int cadence, int duration, EffectType type)
      : isDebuff(isDebuff),
        cadence((int16_t)cadence),
        duration((int16_t)duration),
        fullDuration((int16_t)duration),
        type(type) {}
  virtual ~StatusEffect() = default;
  StatusEffect(const StatusEffect& other) = default;
  StatusEffect& operator=(const StatusEffect& other) = delete;
  [[nodiscard]] virtual StatusEffect* Clone() const = 0;
  inline void Draw(float x, float y, float size) const noexcept {
    DrawTextureScaled(effectToInfo[type].icon, {x, y, size, size}, 0, false, 0, WHITE);
    Util::DrawSwipeCooldownEffect(x + (size == 32 ? 2 : 1), y + (size == 32 ? 2 : 1),
                                  size == 32 ? 28 : 14, fullDuration, duration);
  }
  inline void DrawToolTip(float x, float y) const {
    int lineBreaks = 0;
    auto descriptionText =
        Util::WrapText(GetToolTip(), TOOL_TIP_WIDTH, MINECRAFT_REGULAR, 15, &lineBreaks);

    float toolTipHeight = TOOL_TIP_BASE_HEIGHT + (float)lineBreaks * 15.0F;
    y -= toolTipHeight;
    DrawRectangleRounded({x, y, TOOL_TIP_WIDTH, toolTipHeight}, 0.1F, ROUND_SEGMENTS,
                         Colors::lightGreyMiddleAlpha);
    DrawRectangleRoundedLines({x, y, TOOL_TIP_WIDTH, toolTipHeight}, 0.1F, ROUND_SEGMENTS,
                              2, Colors::darkBackground);
    DrawTextExR(MINECRAFT_BOLD, effectToInfo[type].name.c_str(), {x + 4, y + 3}, 14, 0.5F,
                Colors::darkBackground);

    DrawTextExR(MINECRAFT_ITALIC, descriptionText.c_str(), {x + 4, y + 19}, 14, 0.5F,
                Colors::darkBackground);
  }
  [[nodiscard]] inline bool IsDamageTick() const noexcept {
    return duration > 0 && duration % cadence == 0;
  }

  //Abstract methods
 public:
  [[nodiscard]] virtual inline std::string GetToolTip() const noexcept {
    return Util::CreateEffectToolTipString(effectToInfo[type].description, duration);
  };
  //Called each tick
  virtual inline void TickEffect(EntityStats& stats, const Entity* self) = 0;
  //Called when effect should be applied
  virtual inline void ApplyEffect(EntityStats& stats, const Entity* self) noexcept = 0;
  //Called when effect should be removed
  virtual inline void RemoveEffect(EntityStats& stats, const Entity* self) noexcept = 0;
  //Called when an effect with the same type is added
  virtual inline void AddStack(const StatusEffect* other,
                               const Entity* self) noexcept = 0;
};

#include "effects/StatusEffects.h"

#endif  //MAGEQUEST_SRC_GAMEPLAY_STATUSEFFECT_H_
