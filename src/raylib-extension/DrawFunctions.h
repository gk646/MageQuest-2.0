#ifndef MAGEQUEST_SRC_RAYLIB_EXTENSION_DRAWFUNCTIONS_H_
#define MAGEQUEST_SRC_RAYLIB_EXTENSION_DRAWFUNCTIONS_H_

#include "rlgl.h"

void DrawTextureProFastUltraMirrored(unsigned int id, float drawX, float drawY) {
  rlSetTexture(id);
  rlBegin(RL_QUADS);
  rlColor4ub(255, 255, 255, 255);

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
void DrawTextureFlipped(const Texture2D& texture, float drawX, float drawY, bool flipY) {
  float width = (float)texture.width;
  float height = (float)texture.height;

  // Define texture coordinates
  float leftU = 0.0f;
  float rightU = 1.0f;
  float topV = flipY ? 1.0f : 0.0f;     // flipY controls whether top is 0 or 1
  float bottomV = flipY ? 0.0f : 1.0f;  // flipY controls whether bottom is 1 or 0

  rlSetTexture(texture.id);
  rlBegin(RL_QUADS);

  rlColor4ub(255, 255, 255, 255);  // White tint
  rlNormal3f(0.0f, 0.0f, 1.0f);    // Normal vector pointing towards viewer

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
void DrawTextureScaled(const Texture2D& texture, const RectangleR& dest, float rotation,
                       bool flip, float foff, Color tint) {
  flip = !flip;
  float destX = dest.x;
  if (flip) {
    destX = dest.x + foff;
  }
  // Check if texture is valid
  if (texture.id > 0) {
    auto width = (float)texture.width;
    auto height = (float)texture.height;

    Vector2 topLeft = {0};
    Vector2 topRight = {0};
    Vector2 bottomLeft = {0};
    Vector2 bottomRight = {0};

    // Only calculate rotation if needed
    if (rotation == 0.0f) {
      float x = destX;
      float y = dest.y;
      topLeft = {x, y};
      topRight = {x + dest.width, y};
      bottomLeft = {x, y + dest.height};
      bottomRight = {x + dest.width, y + dest.height};
    } else {
      float sinRotation = sinf(rotation * DEG2RAD);
      float cosRotation = cosf(rotation * DEG2RAD);
      float x = destX;
      float y = dest.y;

      topLeft.x = x + cosRotation - sinRotation;
      topLeft.y = y + sinRotation + cosRotation;

      topRight.x = x + (+dest.width) * cosRotation - sinRotation;
      topRight.y = y + (+dest.width) * sinRotation + cosRotation;

      bottomLeft.x = x + cosRotation - (dest.height) * sinRotation;
      bottomLeft.y = y + sinRotation + (dest.height) * cosRotation;

      bottomRight.x = x + (dest.width) * cosRotation - (dest.height) * sinRotation;
      bottomRight.y = y + (dest.width) * sinRotation + (dest.height) * cosRotation;
    }

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);
    rlNormal3f(0.0f, 0.0f, 1.0f);

    // Top-left corner for texture and quad
    if (flip) rlTexCoord2f(-texture.width / width, 0);
    else
      rlTexCoord2f(0, 0);
    rlVertex2f(topLeft.x, topLeft.y);

    // Bottom-left corner for texture and quad
    if (flip) rlTexCoord2f(-texture.width / width, (texture.height) / height);
    else
      rlTexCoord2f(0, texture.height / height);
    rlVertex2f(bottomLeft.x, bottomLeft.y);

    // Bottom-right corner for texture and quad
    if (flip) rlTexCoord2f(0, (0 + texture.height) / height);
    else
      rlTexCoord2f(-texture.width / width, (texture.height) / height);
    rlVertex2f(bottomRight.x, bottomRight.y);

    // Top-right corner for texture and quad
    if (flip) rlTexCoord2f(0, 0);
    else
      rlTexCoord2f(-texture.width / width, 0);
    rlVertex2f(topRight.x, topRight.y);

    rlEnd();
    rlSetTexture(0);
  }
}
void DrawTextureProFast(const Texture2D& texture, float drawX, float drawY,
                        float rotation, Color tint) {
  if (texture.id > 0) {
    auto width = (float)texture.width;
    auto height = (float)texture.height;

    float topLeftX, topLeftY, topRightX, topRightY, bottomLeftX, bottomLeftY,
        bottomRightX, bottomRightY;

    if (rotation == 0.0f) {
      topLeftX = drawX;
      topLeftY = drawY;
      topRightX = drawX + width;
      topRightY = drawY;
      bottomLeftX = drawX;
      bottomLeftY = drawY + height;
      bottomRightX = drawX + width;
      bottomRightY = drawY + height;
    } else {
      float sinRotation = sinf(rotation * DEG2RAD);
      float cosRotation = cosf(rotation * DEG2RAD);

      topLeftX = drawX;
      topLeftY = drawY;

      topRightX = drawX + width * cosRotation;
      topRightY = drawY + width * sinRotation;

      bottomLeftX = drawX - height * sinRotation;
      bottomLeftY = drawY + height * cosRotation;

      bottomRightX = drawX + width * cosRotation - height * sinRotation;
      bottomRightY = drawY + width * sinRotation + height * cosRotation;
    }

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    rlTexCoord2f(0, 0);
    rlVertex2f(topLeftX, topLeftY);

    rlTexCoord2f(0.0f, 1.0f);
    rlVertex2f(bottomLeftX, bottomLeftY);

    rlTexCoord2f(1.0f, 1.0f);
    rlVertex2f(bottomRightX, bottomRightY);

    rlTexCoord2f(1.0f, 0.0f);
    rlVertex2f(topRightX, topRightY);

    rlEnd();
    rlSetTexture(0);
  }
}
void DrawTextureProFastRot(const Texture2D& texture, float drawX, float drawY,
                           float rotation, Color tint) {
  if (texture.id > 0) {
    float width = texture.width;
    float height = texture.height;

    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    float sinRotation = sinf(rotation * DEG2RAD);
    float cosRotation = cosf(rotation * DEG2RAD);

    float topLeftX =
        drawX + halfWidth - halfWidth * cosRotation + halfHeight * sinRotation;
    float topLeftY =
        drawY + halfHeight - halfWidth * sinRotation - halfHeight * cosRotation;

    float topRightX =
        drawX + halfWidth + halfWidth * cosRotation + halfHeight * sinRotation;
    float topRightY =
        drawY + halfHeight + halfWidth * sinRotation - halfHeight * cosRotation;

    float bottomLeftX =
        drawX + halfWidth - halfWidth * cosRotation - halfHeight * sinRotation;
    float bottomLeftY =
        drawY + halfHeight - halfWidth * sinRotation + halfHeight * cosRotation;

    float bottomRightX =
        drawX + halfWidth + halfWidth * cosRotation - halfHeight * sinRotation;
    float bottomRightY =
        drawY + halfHeight + halfWidth * sinRotation + halfHeight * cosRotation;

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    rlTexCoord2f(0.0f, 0.0f);
    rlVertex2f(topLeftX, topLeftY);

    rlTexCoord2f(0.0f, 1.0f);
    rlVertex2f(bottomLeftX, bottomLeftY);

    rlTexCoord2f(1.0f, 1.0f);
    rlVertex2f(bottomRightX, bottomRightY);

    rlTexCoord2f(1.0f, 0.0f);
    rlVertex2f(topRightX, topRightY);

    rlEnd();
    rlSetTexture(0);
  }
}
void DrawTextureProFastRotOffset(const Texture2D& texture, float drawX, float drawY,
                                 float rotation, Color tint, float offsetX,
                                 float offsetY) {
  if (texture.id > 0) {
    float width = texture.width;
    float height = texture.height;

    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    float sinRotation = sinf(rotation * DEG2RAD);
    float cosRotation = cosf(rotation * DEG2RAD);

    // Apply the constant offset
    drawX += offsetX;
    drawY += offsetY;

    float topLeftX =
        drawX + halfWidth - halfWidth * cosRotation + halfHeight * sinRotation;
    float topLeftY =
        drawY + halfHeight - halfWidth * sinRotation - halfHeight * cosRotation;

    float topRightX =
        drawX + halfWidth + halfWidth * cosRotation + halfHeight * sinRotation;
    float topRightY =
        drawY + halfHeight + halfWidth * sinRotation - halfHeight * cosRotation;

    float bottomLeftX =
        drawX + halfWidth - halfWidth * cosRotation - halfHeight * sinRotation;
    float bottomLeftY =
        drawY + halfHeight - halfWidth * sinRotation + halfHeight * cosRotation;

    float bottomRightX =
        drawX + halfWidth + halfWidth * cosRotation - halfHeight * sinRotation;
    float bottomRightY =
        drawY + halfHeight + halfWidth * sinRotation + halfHeight * cosRotation;

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);
    rlNormal3f(0.0f, 0.0f, 1.0f);

    rlTexCoord2f(0.0f, 0.0f);
    rlVertex2f(topLeftX, topLeftY);

    rlTexCoord2f(0.0f, 1.0f);
    rlVertex2f(bottomLeftX, bottomLeftY);

    rlTexCoord2f(1.0f, 1.0f);
    rlVertex2f(bottomRightX, bottomRightY);

    rlTexCoord2f(1.0f, 0.0f);
    rlVertex2f(topRightX, topRightY);

    rlEnd();
    rlSetTexture(0);
  }
}

void DrawTextureProFastEx(const Texture2D& texture, float x, float y, float flip_offset_x,
                          float rotation, bool flipped, Color tint) {
  if (texture.id > 0) {
    auto width = (float)texture.width;
    auto height = (float)texture.height;

    Vector2 topLeft = {0};
    Vector2 topRight = {0};
    Vector2 bottomLeft = {0};
    Vector2 bottomRight = {0};

    if (flipped) x += flip_offset_x;
    if (rotation == 0.0f) {
      topLeft = {x, y};
      topRight = {x + width, y};
      bottomLeft = {x, y + height};
      bottomRight = {x + width, y + height};
    } else {
      float sinRotation = sinf(rotation * DEG2RAD);
      float cosRotation = cosf(rotation * DEG2RAD);

      topLeft.x = x;
      topLeft.y = y;

      topRight.x = x + width * cosRotation;
      topRight.y = y + width * sinRotation;

      bottomLeft.x = x - height * sinRotation;
      bottomLeft.y = y + height * cosRotation;

      bottomRight.x = x + width * cosRotation - height * sinRotation;
      bottomRight.y = y + width * sinRotation + height * cosRotation;
    }

    rlSetTexture(texture.id);
    rlBegin(RL_QUADS);

    rlColor4ub(tint.r, tint.g, tint.b, tint.a);
    rlNormal3f(0.0f, 0.0f, 1.0f);  // Normal vector pointing towards viewer

    // Top-left corner for texture and quad
    if (flipped) rlTexCoord2f(1.0f, 0.0f);
    else
      rlTexCoord2f(0.0f, 0.0f);
    rlVertex2f(topLeft.x, topLeft.y);

    // Bottom-left corner for texture and quad
    if (flipped) rlTexCoord2f(1.0f, 1.0f);
    else
      rlTexCoord2f(0.0f, 1.0f);
    rlVertex2f(bottomLeft.x, bottomLeft.y);

    // Bottom-right corner for texture and quad
    if (flipped) rlTexCoord2f(0.0f, 1.0f);
    else
      rlTexCoord2f(1.0f, 1.0f);
    rlVertex2f(bottomRight.x, bottomRight.y);

    // Top-right corner for texture and quad
    if (flipped) rlTexCoord2f(0.0f, 0.0f);
    else
      rlTexCoord2f(1.0f, 0.0f);
    rlVertex2f(topRight.x, topRight.y);

    rlEnd();
    rlSetTexture(0);
  }
}
void DrawTextureProFastUltra(unsigned int id, float drawX, float drawY) {
  rlSetTexture(id);
  rlBegin(RL_QUADS);
  rlColor4ub(255, 255, 255, 255);

  rlTexCoord2f(0.0f, 0.0f);
  rlVertex2f(drawX, drawY);

  rlTexCoord2f(0.0f, 1.0f);
  rlVertex2f(drawX, drawY + 48);

  rlTexCoord2f(1.0f, 1.0f);
  rlVertex2f(drawX + 48, drawY + 48);

  rlTexCoord2f(1.0f, 0.0f);
  rlVertex2f(drawX + 48, drawY);

  rlEnd();
  rlSetTexture(0);
}
void DrawRectOutlineMiddleRotation(const RectangleR& rect, float rotation, Color color) {

  float halfWidth = rect.width * 0.5f;
  float halfHeight = rect.height * 0.5f;

  float sinRotation = sinf(rotation * DEG2RAD);
  float cosRotation = cosf(rotation * DEG2RAD);

  float topLeftX = rect.x + halfWidth - halfWidth * cosRotation + halfHeight * sinRotation;
  float topLeftY = rect.y + halfHeight - halfWidth * sinRotation - halfHeight * cosRotation;

  float topRightX = rect.x + halfWidth + halfWidth * cosRotation + halfHeight * sinRotation;
  float topRightY = rect.y + halfHeight + halfWidth * sinRotation - halfHeight * cosRotation;

  float bottomLeftX = rect.x + halfWidth - halfWidth * cosRotation - halfHeight * sinRotation;
  float bottomLeftY = rect.y + halfHeight - halfWidth * sinRotation + halfHeight * cosRotation;

  float bottomRightX = rect.x + halfWidth + halfWidth * cosRotation - halfHeight * sinRotation;
  float bottomRightY = rect.y + halfHeight + halfWidth * sinRotation + halfHeight * cosRotation;

  rlBegin(RL_LINES);
  rlColor4ub(color.r, color.g, color.b, color.a);

  // Top line
  rlVertex2f(topLeftX, topLeftY);
  rlVertex2f(topRightX, topRightY);

  // Bottom line
  rlVertex2f(bottomLeftX, bottomLeftY);
  rlVertex2f(bottomRightX, bottomRightY);

  // Left line
  rlVertex2f(topLeftX, topLeftY);
  rlVertex2f(bottomLeftX, bottomLeftY);

  // Right line
  rlVertex2f(topRightX, topRightY);
  rlVertex2f(bottomRightX, bottomRightY);

  rlEnd();
}

#endif  //MAGEQUEST_SRC_RAYLIB_EXTENSION_DRAWFUNCTIONS_H_
