#ifndef MAGEQUEST_SRC_GRAPHICS_GIFDRAWER_H_
#define MAGEQUEST_SRC_GRAPHICS_GIFDRAWER_H_

struct GifDrawer {
  int anim_frames = 0;
  Image images{};
  Texture2D texture{};
  int currentAnimFrame = 0;
  int frameDelay = 10;
  int frameCounter = 0;
  int frame_count = 0;
  bool finished = false;
  explicit GifDrawer(const std::string& path) {
    images = LoadImageAnim(path.c_str(), &anim_frames);
    texture = LoadTextureFromImage(images);
  }
  ~GifDrawer() {
    UnloadImage(images);
    UnloadTexture(texture);
  }
  void draw() {
    if (finished) return;

    frameCounter++;
    if (frameCounter >= frameDelay) {
      currentAnimFrame++;
      frame_count++;
      if (currentAnimFrame >= anim_frames) {
        finished = true;
        return;
      }
      unsigned int nextFrameDataOffset =
          images.width * images.height * 4 * currentAnimFrame;

      UpdateTexture(texture, ((unsigned char*)images.data) + nextFrameDataOffset);

      frameCounter = 0;
    }
    DrawTextureProFast(texture, GetScreenWidth() / 2.0F - texture.width / 2.0F,
                       GetScreenHeight() / 2.0F - texture.height / 2.0F, 0, WHITE);

    if (frame_count == 5) {
      frameDelay = 6;
    } else if (frame_count == 51) {
      frameDelay = 15;
    } else if (frame_count == 57) {
      frameDelay = 40;
    } else if (frame_count == 59) {
      frameDelay = 7;
    }
  }
};

#endif  //MAGEQUEST_SRC_GRAPHICS_GIFDRAWER_H_
