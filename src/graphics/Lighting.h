#ifndef MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
#define MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
namespace Lighting {
struct SpotLightInfo {
  float innerRadius;
  float outerRadius;
  Vector3 lightColors;
};
inline static std::unordered_map<ProjectileType, SpotLightInfo> typeToLight{
    {FIRE_BALL, {15, 100, {0.8745f, 0.2431f, 0.1373f} }},
    {ENERGY_SPHERE, {20, 100, {0.3804f, 0.6588f, 0.8902f}}}};
inline static int spotLightCount = 0;
inline static constexpr uint16_t FULL_DAY_TICKS = 15 * 60 * 60;
inline static uint16_t dayTicks = 0;
inline static Color currentDayFade{24, 20, 37, 0};
inline static uint8_t fadeAlpha = 150;

namespace Shaders {
inline static Shader spotLight;
inline static int SPOT_LIGHT_RADIUS;
inline static int SPOT_LIGHT_POSITION;
inline static int SPOT_LIGHT_COLOR;
inline static int SPOT_LIGHT_INNER_RADIUS;
inline static int SPOT_LIGHT_NUM;
Vector2 lightPositions[MAX_DYNAMIC_LIGHTS];
float innerRadii[MAX_DYNAMIC_LIGHTS];
float outerRadii[MAX_DYNAMIC_LIGHTS];
Vector3 lightColors[MAX_DYNAMIC_LIGHTS];
}  // namespace Shaders

uint8_t CalculateAlpha() {
  double phaseOfDay = (2 * PI * (float)dayTicks) / FULL_DAY_TICKS;
  double alphaDouble = (std::sin(phaseOfDay - (PI / 2)) + 1) / 2;
  return static_cast<uint8_t>(210 * alphaDouble);
}
inline static void StartDynamicLights() noexcept {
  int i = 0;
  for (auto p : PROJECTILES) {
    if (i >= MAX_DYNAMIC_LIGHTS) break;
    if (p->active) {
      Shaders::lightPositions[i] = {p->pos.x_+DRAW_X,SCREEN_HEIGHT- (p->pos.y_+DRAW_Y)};
      Shaders::innerRadii[i] = typeToLight[p->projectileType].innerRadius;
      Shaders::outerRadii[i] = typeToLight[p->projectileType].outerRadius;
      Shaders::lightColors[i] = typeToLight[p->projectileType].lightColors;
      i++;
    }
  }
  spotLightCount = i;
  SetShaderValueV(Shaders::spotLight,Shaders::SPOT_LIGHT_POSITION,Shaders::lightPositions,SHADER_UNIFORM_VEC2,spotLightCount);
  SetShaderValueV(Shaders::spotLight,Shaders::SPOT_LIGHT_COLOR,Shaders::lightColors,SHADER_UNIFORM_VEC3,spotLightCount);
  SetShaderValueV(Shaders::spotLight,Shaders::SPOT_LIGHT_RADIUS,Shaders::outerRadii,SHADER_UNIFORM_FLOAT,spotLightCount);
  SetShaderValueV(Shaders::spotLight,Shaders::SPOT_LIGHT_INNER_RADIUS,Shaders::innerRadii,SHADER_UNIFORM_FLOAT,spotLightCount);
  SetShaderValue(Shaders::spotLight, Shaders::SPOT_LIGHT_NUM, &spotLightCount,
                 SHADER_UNIFORM_INT);

  BeginShaderMode(Shaders::spotLight);
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
  currentDayFade.a = CalculateAlpha();
}
}  // namespace Lighting
#endif  //MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
