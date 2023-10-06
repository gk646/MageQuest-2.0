#ifndef MAGEQUEST_SRC_RAYLIB_EXTENSION_DRAWFUNCTIONS_H_
#define MAGEQUEST_SRC_RAYLIB_EXTENSION_DRAWFUNCTIONS_H_

#include "rlgl.h"

void DrawTextureProFastUltraMirrored(unsigned int id, float drawX, float drawY)
{
  rlSetTexture(id);
  rlBegin(RL_QUADS);
  rlColor4ub(255,255,255,255);

  rlTexCoord2f(0.0f, 1.0f);
  rlVertex2f(drawX, drawY);

  rlTexCoord2f(0.0f, 0.0f);
  rlVertex2f(drawX, drawY + 48);

  rlTexCoord2f(1.0f, 0.0f);
  rlVertex2f(drawX + 48, drawY + 48);

  rlTexCoord2f(1.0f, 1.0f);
  rlVertex2f(drawX + 48, drawY);

  rlEnd();
}
void DrawTextureFlipped(Texture2D texture, float drawX, float drawY, bool flipY) {
  float width = (float)texture.width;
  float height = (float)texture.height;

  // Define texture coordinates
  float leftU = 0.0f;
  float rightU = 1.0f;
  float topV = flipY ? 1.0f : 0.0f; // flipY controls whether top is 0 or 1
  float bottomV = flipY ? 0.0f : 1.0f; // flipY controls whether bottom is 1 or 0

  rlSetTexture(texture.id);
  rlBegin(RL_QUADS);

  rlColor4ub(255, 255, 255, 255); // White tint
  rlNormal3f(0.0f, 0.0f, 1.0f); // Normal vector pointing towards viewer

  // Top-left corner for texture and quad
  rlTexCoord2f(leftU, topV);
  rlVertex2f(drawX, drawY);

  // Bottom-left corner for texture and quad
  rlTexCoord2f(leftU, bottomV);
  rlVertex2f(drawX, drawY + height);

  // Bottom-right corner for texture and quad
  rlTexCoord2f(rightU, bottomV);
  rlVertex2f(drawX + width, drawY + height);

  // Top-right corner for texture and quad
  rlTexCoord2f(rightU, topV);
  rlVertex2f(drawX + width, drawY);

  rlEnd();
  rlSetTexture(0);
}
#endif  //MAGEQUEST_SRC_RAYLIB_EXTENSION_DRAWFUNCTIONS_H_
