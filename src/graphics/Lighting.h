#ifndef MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
#define MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
namespace Lighting {
struct SpotLightInfo {
  float innerRadius;
  float outerRadius;
  Vector3 lightColors;
};

inline static std::unordered_map<ProjectileType, SpotLightInfo> typeToLight{
    {FIRE_BALL, {7, 120, {0.8745f, 0.2431f, 0.1373f}}},
    {ENERGY_SPHERE, {15, 200, {0.3804f, 0.6588f, 0.8902f}}}};

inline static constexpr uint16_t FULL_DAY_TICKS = UINT16_MAX;
inline static uint16_t dayTicks = 18000;
inline static float currentNightAlpha = 0;
inline static uint8_t fadeAlpha = 150;

namespace Shaders {
inline static Shader spotLight;
inline static Shader nightShader;
inline static Shader postProcessing;
inline static int SPOT_LIGHT_RADIUS;
inline static int SPOT_LIGHT_POSITION;
inline static int SPOT_LIGHT_COLOR;
inline static int SPOT_LIGHT_INNER_RADIUS;
inline static int SPOT_LIGHT_NUM;
inline static int SPOT_LIGHT_TIME;
inline static int SPOT_LIGHT_ALPHA;
inline static int NIGHT_SHADER_ALPHA;
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
  SetShaderValue(spotLight, SPOT_LIGHT_ALPHA, &currentNightAlpha, SHADER_UNIFORM_FLOAT);
  BeginShaderMode(spotLight);
}
inline static void StartNightShader() noexcept {
  SetShaderValue(nightShader, NIGHT_SHADER_ALPHA, &currentNightAlpha,
                 SHADER_UNIFORM_FLOAT);
  BeginShaderMode(nightShader);
}
inline static void StartPostProcessing() noexcept {
  BeginShaderMode(postProcessing);
}
}  // namespace Shaders

namespace AmbientOcclusion {
std::vector<Vector3 > CURRENT_SHADOW_MAP;
inline static void GenerateShadowMap() noexcept {
  CURRENT_SHADOW_MAP.clear();
  for (int_fast16_t j = 0; j < CURRENT_MAP_SIZE; j++) {
    for (int_fast16_t i = 0; i < CURRENT_MAP_SIZE; i++) {
      if (CheckTileCollision(i, j)) {
        if (i == 0 || !CheckTileCollision(i - 1, j)) {
          int_fast16_t height = 1;
          while (i + height < CURRENT_MAP_SIZE && CheckTileCollision(i + height, j)) {
            height++;
          }
          CURRENT_SHADOW_MAP.emplace_back(i, j, height);
          i += height - 1;
        }
      }
    }
  }
}
}  // namespace AmbientOcclusion
inline static float CalculateAlpha() noexcept {
  float phaseOfDay = (2 * PI * (float)dayTicks) / FULL_DAY_TICKS;
  float alphaDouble = (std::sin(phaseOfDay - (PI / 2)) + 1) / 2;
  return alphaDouble * 0.95F;
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
  dayTicks++;
  if (dayTicks >= FULL_DAY_TICKS) {
    dayTicks = 0;
  }
  currentNightAlpha = CalculateAlpha();
}
}  // namespace Lighting
#endif  //MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
