#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_SATIRO_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_SATIRO_H_
struct Satiro final : public NPC {
  Satiro(float x, float y, Zone zone)
      : NPC({x, y}, &textures::npc::SATIRO, zone, NPC_ID::SATIRO, 2) {}
  void Draw() final {
    NPC::Draw();
    if (actionState == 1) {
      DrawSpecial();
    } else {
      if (moving) {
        DrawTextureProFastEx(resource->walk[spriteCounter % 104 / 13], pos.x_ + DRAW_X,
                             pos.y_ + DRAW_Y, 0, 0, isFlipped, WHITE);

      } else {
        DrawTextureProFastEx(resource->idle[spriteCounter % 78 / 13],
                             pos.x_ + DRAW_X - 20, pos.y_ + DRAW_Y - 18, 0, 0, isFlipped,
                             WHITE);
      }
    }

    DRAW_HITBOXES();
  }
  void Update() final {
    NPC::Update();
    INTERACT_WITH_PLAYER()
  }
  inline void DrawSpecial() noexcept {
    int num = spriteCounter % 150 / 15;
    if (num < 9) {
      DrawTextureProFastEx(resource->special[num], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, 0, 0,
                           isFlipped, WHITE);
    } else {
      actionState = 0;
    }
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_SATIRO_H_
