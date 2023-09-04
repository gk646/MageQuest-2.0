#ifndef MAGEQUEST_SRC_GRAPHICS_GIFDRAWER_H_
#define MAGEQUEST_SRC_GRAPHICS_GIFDRAWER_H_

struct GifDrawer {
  int anim_frames = 0;
  Image images{};
  Texture2D texture{};
  int currentAnimFrame = 0;
  int frameDelay = 10;
  int frameCounter = 0;
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
    if(finished) return;

    frameCounter++;
    if (frameCounter >= frameDelay) {
      currentAnimFrame++;
      if (currentAnimFrame >= anim_frames) {
        finished = true;
        return;
      }
      unsigned int nextFrameDataOffset = images.width * images.height * 4 * currentAnimFrame;

      UpdateTexture(texture, ((unsigned char*)images.data) + nextFrameDataOffset);

      frameCounter = 0;
    }
    DrawTexture(texture, GetScreenWidth() / 2 - texture.width / 2, GetScreenHeight()/2- texture.height / 2, WHITE);
  }
};

#endif  //MAGEQUEST_SRC_GRAPHICS_GIFDRAWER_H_
