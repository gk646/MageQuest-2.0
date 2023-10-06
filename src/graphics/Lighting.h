#ifndef MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
#define MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
namespace Lighting {
struct SpotLightInfo {
  float innerRadius;
  float outerRadius;
  Vector3 lightColors;
};
inline static std::unordered_map<ProjectileType, SpotLightInfo> typeToLight{
    {FIRE_BALL, {7, 75, {0.8745f, 0.2431f, 0.1373f}}},
    {ENERGY_SPHERE, {15, 150, {0.3804f, 0.6588f, 0.8902f}}}};

inline static constexpr uint16_t FULL_DAY_TICKS = 15 * 60 * 60;
inline static uint16_t dayTicks = 0;
inline static Color currentDayFade{24, 20, 37, 204};
inline static uint8_t fadeAlpha = 150;

namespace Shaders {
inline static Shader spotLight;
inline static int SPOT_LIGHT_RADIUS;
inline static int SPOT_LIGHT_POSITION;
inline static int SPOT_LIGHT_COLOR;
inline static int SPOT_LIGHT_INNER_RADIUS;
inline static int SPOT_LIGHT_NUM;
inline static int SPOT_LIGHT_TIME;
inline static int spotLightCount = 0;
inline static float spotLightTime = 0;
Vector2 lightPositions[MAX_DYNAMIC_LIGHTS];
float innerRadii[MAX_DYNAMIC_LIGHTS];
float outerRadii[MAX_DYNAMIC_LIGHTS];
Vector3 lightColors[MAX_DYNAMIC_LIGHTS];
inline static void StartDynamicLights() noexcept {
  int i = 0;
  for (auto p : PROJECTILES) {
    if (i >= MAX_DYNAMIC_LIGHTS) break;
    if (p->active) {
      lightPositions[i] = {p->pos.x_ + DRAW_X + p->size.x_ / 2,
                           SCREEN_HEIGHT - (p->pos.y_ + DRAW_Y + p->size.y_ / 2)};
      innerRadii[i] = typeToLight[p->projectileType].innerRadius;
      outerRadii[i] = typeToLight[p->projectileType].outerRadius;
      lightColors[i] = typeToLight[p->projectileType].lightColors;
      i++;
    }
  }
  spotLightTime += 0.0083;
  spotLightCount = i;
  SetShaderValueV(spotLight, SPOT_LIGHT_POSITION, lightPositions, SHADER_UNIFORM_VEC2,
                  spotLightCount);
  SetShaderValueV(spotLight, SPOT_LIGHT_COLOR, lightColors, SHADER_UNIFORM_VEC3,
                  spotLightCount);
  SetShaderValueV(spotLight, SPOT_LIGHT_RADIUS, outerRadii, SHADER_UNIFORM_FLOAT,
                  spotLightCount);
  SetShaderValueV(spotLight, SPOT_LIGHT_INNER_RADIUS, innerRadii, SHADER_UNIFORM_FLOAT,
                  spotLightCount);
  SetShaderValue(spotLight, SPOT_LIGHT_NUM, &spotLightCount, SHADER_UNIFORM_INT);
  SetShaderValue(spotLight, SPOT_LIGHT_TIME, &spotLightTime, SHADER_UNIFORM_FLOAT);

  BeginShaderMode(spotLight);
}
}  // namespace Shaders

uint8_t CalculateAlpha() {
  double phaseOfDay = (2 * PI * (float)dayTicks) / FULL_DAY_TICKS;
  double alphaDouble = (std::sin(phaseOfDay - (PI / 2)) + 1) / 2;
  return static_cast<uint8_t>(210 * alphaDouble);
}
inline static void DrawFade() noexcept {
  DrawRectangleProFast(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {24, 20, 37, fadeAlpha});
}
inline static void DrawScreenEffects() noexcept {
  if (fadeAlpha < 255) {
    DrawFade();
  }
}
inline static void UpdateScreenEffects() noexcept {
  if (fadeAlpha < 255) {
    fadeAlpha--;
  }
  dayTicks = (dayTicks + 1) % FULL_DAY_TICKS;
  //currentDayFade.a = CalculateAlpha();
}
}  // namespace Lighting
#endif  //MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
