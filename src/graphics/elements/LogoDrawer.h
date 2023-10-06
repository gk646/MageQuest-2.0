#ifndef MAGEQUEST_SRC_GRAPHICS_LOGODRAWER_H_
#define MAGEQUEST_SRC_GRAPHICS_LOGODRAWER_H_

struct LogoDrawer {
  static constexpr float speed = 0.9;
  float transparency = 0;
  Texture texture;
  int finished = 0;
  LogoDrawer(const std::string& name) : texture(LoadTexture(name.c_str())) {}
  ~LogoDrawer() { UnloadTexture(texture); }

  void draw() noexcept {
    DrawTextureProFast(texture, GetScreenWidth() / 2 - texture.width/2,
                       GetScreenHeight() / 2 - texture.height/2, 0,
                       {255, 255, 255, (uint8_t)transparency});

    if(finished == 0){
      if(transparency < 254){
        transparency+=speed;
      }else{
        finished = 1;
      }
    }else {
      if(transparency > 1){
        transparency-= speed;
      }else{
        finished = -1;
      }
    }
  }
};
#endif  //MAGEQUEST_SRC_GRAPHICS_LOGODRAWER_H_
