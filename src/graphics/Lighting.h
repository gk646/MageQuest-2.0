#ifndef MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
#define MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
namespace Lighting {
struct SpotLightInfo {
  float innerRadius;
  float outerRadius;
  Vector3 lightColors;
};
struct ShadowObject {
  uint16_t xTile;
  uint16_t yTile;
  ShadowType type;
  [[nodiscard]] inline float x() const noexcept { return xTile; }
  [[nodiscard]] inline float y() const noexcept { return yTile; }
};

inline static std::unordered_map<ProjectileType, SpotLightInfo> typeToLight{
    {FIRE_BALL, {7, 120, {0.8745f, 0.2431f, 0.1373f}}},
    {ENERGY_SPHERE, {15, 200, {0.3804f, 0.6588f, 0.8902f}}}};

inline static constexpr uint16_t FULL_DAY_TICKS = UINT16_MAX;
inline static uint16_t dayTicks = 0;
inline static float currentNightAlpha = 0;
inline static uint8_t fadeAlpha = 150;
namespace AmbientOcclusion {
QuadTree<ShadowObject> CURRENT_SHADOW_TREE{{0, 0, 0, 0}};
inline static void GenerateShadowMap() noexcept {
  CURRENT_SHADOW_TREE.clear();
  CURRENT_SHADOW_TREE.set_bounds(
      {0.0F, 0.0F, (float)CURRENT_MAP_SIZE, (float)CURRENT_MAP_SIZE});
  for (uint16_t j = 0; j < CURRENT_MAP_SIZE; j++) {
    for (uint16_t i = 0; i < CURRENT_MAP_SIZE; i++) {
      for (const int16_t shadowTileNumber : shadowTileNumbers) {
        if (CURRENT_MIDDLE_GROUND[i][j] == shadowTileNumber) {
          CURRENT_SHADOW_TREE.insert(ShadowObject{i, j, ShadowType(shadowTileNumber)});
        }
      }
    }
  }
}
inline static void DrawAmbientOcclusion() noexcept {
  int var = 255 - currentNightAlpha * 270;
  unsigned char alpha = 0;
  if (var > 0) {
    alpha = (unsigned char)var;
  }
  for (const auto& obj : CURRENT_SHADOW_TREE.get_subrect(
           {PLAYER_TILE->x - SCREEN_TILE_WIDTH / 2.0F,
            PLAYER_TILE->y - SCREEN_TILE_HEIGHT / 2.0F - 4, (float)SCREEN_TILE_WIDTH,
            (float)SCREEN_TILE_HEIGHT + 4})) {
    PointI tilePos = {obj->xTile * 48, obj->yTile * 48 + 48};
    auto& tex = shadowToTexture[obj->type];

    DrawTextureProFastRotOffset(tex, 24.0F + tilePos.x + DRAW_X - tex.width / 2,
                                tilePos.y + DRAW_Y - 12.0F, 0, {255, 255, 255, alpha}, 0,
                                0);
  }
}
inline static Vector2 CalculateShadowDirection() noexcept {
  float phaseOfDay = (2 * PI * (float)dayTicks) / FULL_DAY_TICKS;

  float shadowX = cos(phaseOfDay);
  float shadowY = sin(phaseOfDay);

  return Vector2(shadowX, shadowY);
}
}  // namespace AmbientOcclusion
namespace Shaders {
inline static bool lightOn = false;
inline static Vector2* cameraVec = new Vector2{0, 0};
inline static Shader spotLight;
inline static Shader nightShader;
inline static Shader postProcessing;
inline static int SPOT_LIGHT_RADIUS;
inline static int SPOT_LIGHT_POSITION;
inline static int SPOT_LIGHT_COLOR;
inline static int SPOT_LIGHT_INNER_RADIUS;
inline static int SPOT_LIGHT_NUM;
inline static int SPOT_LIGHT_TIME;
inline static int SPOT_LIGHT_PLAYER;
inline static int SPOT_LIGHT_ALPHA;
inline static int NIGHT_SHADER_ALPHA;
inline static int NIGHT_SHADER_PLAYER;
inline static int spotLightCount = 0;
inline static float spotLightTime = 0;
Vector2 lightPositions[MAX_DYNAMIC_LIGHTS];
float innerRadii[MAX_DYNAMIC_LIGHTS];
float outerRadii[MAX_DYNAMIC_LIGHTS];
Vector3 lightColors[MAX_DYNAMIC_LIGHTS];
inline static void UpdateDynamicLights() noexcept {
  int i = 0;
  for (auto p : PROJECTILES) {
    if (i >= MAX_DYNAMIC_LIGHTS) break;
    if (p->active && p->illuminated) {
      lightPositions[i] = {p->pos.x_ + DRAW_X + p->size.x_ / 2,
                           SCREEN_HEIGHT - (p->pos.y_ + DRAW_Y + p->size.y_ / 2)};
      innerRadii[i] = typeToLight[p->projectileType].innerRadius;
      outerRadii[i] = typeToLight[p->projectileType].outerRadius;
      lightColors[i] = typeToLight[p->projectileType].lightColors;
      i++;
    }
  }

  spotLightCount = i;
}
inline static void StartDynamicLights() noexcept {
  spotLightTime += 0.0083;
  if (IsKeyPressed(PLAYER_KEYBINDS[(int)Keybind::PLAYER_LIGHT])) lightOn = !lightOn;
  if (lightOn) {
    cameraVec->x = CAMERA_X;
    cameraVec->y = CAMERA_Y;
  } else {
    cameraVec->x = 0;
    cameraVec->y = 0;
  }

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
  SetShaderValue(spotLight, SPOT_LIGHT_PLAYER, cameraVec, SHADER_UNIFORM_VEC2);
  BeginShaderMode(spotLight);
}
inline static void StartNightShader() noexcept {
  SetShaderValue(nightShader, NIGHT_SHADER_ALPHA, &currentNightAlpha,
                 SHADER_UNIFORM_FLOAT);
  SetShaderValue(nightShader, NIGHT_SHADER_PLAYER, cameraVec, SHADER_UNIFORM_VEC2);
  BeginShaderMode(nightShader);
}
inline static void StartPostProcessing() noexcept {
  BeginShaderMode(postProcessing);
}
}  // namespace Shaders
inline static float CalculateAlpha() noexcept {
  float phaseOfDay = (2 * PI * (float)dayTicks) / FULL_DAY_TICKS;
  float alphaDouble = (std::sin(phaseOfDay - (PI / 2)) + 1) / 2;
  return alphaDouble * 0.99F;
}
inline static void DrawScreenEffects() noexcept {
  if (fadeAlpha < 255) {
    DrawRectangleProFast(
        0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
        {24, 20, 37,
         std::max((unsigned char)std::max(fadeAlpha - 50, 0), (unsigned char)0)});
    Util::DrawCenteredText(ANT_PARTY, 40, zoneMap[CURRENT_ZONE].c_str(), SCREEN_WIDTH / 2,
                           SCREEN_HEIGHT * 0.3F,
                           {252, 252, 252, static_cast<unsigned char>(255 - fadeAlpha)});
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
  Lighting::Shaders::UpdateDynamicLights();
}
}  // namespace Lighting
#endif  //MAGEQUEST_SRC_GRAPHICS_LIGHTING_H_
