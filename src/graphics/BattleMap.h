#ifndef DND_SRC_UTIL_BATTLEMAP_H_
#define DND_SRC_UTIL_BATTLEMAP_H_

class BattleMap {
  Vector2 imagePosition;
  Rectangle sourceRec;
  Vector2 origin = {0, 0};
  Texture2D back_ground;
  Vector2 grid;

 public:
  float zoom = 1;
  Rectangle destRec;

  BattleMap() {}
  ~BattleMap() {}

  void load(int screen_width, int screen_height, std::string path, Vector2 grid) {
    this->grid = grid;
    back_ground = LoadTexture(path.c_str());
    imagePosition = {(float)(screen_width - back_ground.width) / 2,
                     (float)(screen_height - back_ground.height) / 2};
    sourceRec = {0, 0, (float)back_ground.width, (float)back_ground.height};
    destRec = {imagePosition.x, imagePosition.y, (float)back_ground.width,
               (float)back_ground.height};
  }

  void draw(int screen_width, int screen_height, Point camera_pos) {
    int mouseWheelMove = GetMouseWheelMove();
    float previousZoom = zoom;
    zoom += mouseWheelMove * 0.05f;

    if (zoom <= 0.1f) {
      zoom = 0.1f;
    }

    destRec.width = back_ground.width * zoom;
    destRec.height = back_ground.height * zoom;

    // These calculations keep the zoom focused on the screen center
    destRec.x = (screen_width / 2) - (back_ground.width * zoom) / 2 + camera_pos.x() * zoom;
    destRec.y = (screen_height / 2) - (back_ground.height * zoom) / 2 + camera_pos.y() * zoom;

    DrawTexturePro(back_ground, sourceRec, destRec, origin, 0.0f, WHITE);
  }


  float getMapScale() {
    float xratio = back_ground.width / grid.x;
    float yratio = back_ground.height / grid.y;
    if (xratio != yratio) {
      return (yratio + xratio) / 2;
    }
    return xratio;
  }
};

#endif  //DND_SRC_UTIL_BATTLEMAP_H_
