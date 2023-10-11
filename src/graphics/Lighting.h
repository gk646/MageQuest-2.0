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
      if (CheckTileCollision(i, j)) {
        if (i == 0 || !CheckTileCollision(i - 1, j)) {
          uint8_t height = 1;
          while (i + height < CURRENT_MAP_SIZE && CheckTileCollision(i + height, j)) {
            height++;
          }
          //CURRENT_SHADOW_TREE.insert({i, j, true, height});
          i += height - 1;
        }
      }
    }
  }
}
inline static void DrawAmbientOcclusion() noexcept {
  return;
  for (auto obj : CURRENT_SHADOW_TREE.get_subrect(
           {PLAYER_TILE->x - SCREEN_TILE_WIDTH / 2.0F,
            PLAYER_TILE->y - SCREEN_TILE_HEIGHT / 2.0F, (float)SCREEN_TILE_WIDTH,
            (float)SCREEN_TILE_HEIGHT})) {
    // Get the object's position and height
    PointI tilePos = {obj->xTile * 48, obj->yTile * 48 + 48};

    // Calculate shadow direction and length based on time
    float time = 0.9;         // normalized time between 0 and 1
    float shadowLength = 48;  // for example
    Vector2 shadowDirection = Vector2(cos(time * 2 * PI), sin(time * 2 * PI));

    // Calculate four corners of the shadow rhombus
    Vector2 baseLeft = Vector2(tilePos.x + DRAW_X, tilePos.y + DRAW_Y);
    Vector2 baseRight = Vector2(tilePos.x + TILE_SIZE + DRAW_X, tilePos.y + DRAW_Y);
    Vector2 tipLeft = {baseLeft.x - shadowDirection.x * shadowLength,
                       baseLeft.y - shadowDirection.y * shadowLength};
    Vector2 tipRight = {baseRight.x - shadowDirection.x * shadowLength,
                        baseRight.y - shadowDirection.y * shadowLength};

    DrawTriangle(baseLeft, tipLeft, baseRight, {50, 50, 50, 150});

    DrawTriangle(baseRight, tipLeft, tipRight, {255, 0, 0, 150});
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
    cameraVec->x = CAMERA_X - 10;
    cameraVec->y = CAMERA_Y - 24;
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
    DrawRectangleProFast(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, {24, 20, 37, fadeAlpha});
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
